#include "DatosEmpleadosOptions.h"
#include "ui_DatosEmpleadosOptions.h"

#include <QDebug>

#include <QSqlTableModel>

#include "DataBaseUtils.h"
#include "NotificationSender.h"

using namespace DataBaseUtils;

const int DNI_COLUMN = 0;
const int ID_COLUMN = 1;
const int NAME_COLUMN = 2;
const int ADMIN_COLUMN = 3;

QWidget*
MyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& aIndex) const
{
    QWidget* editor;
    if (aIndex.column() == ADMIN_COLUMN) {
        QCheckBox* checkbox = new QCheckBox(parent);
        checkbox->setChecked(false);
        editor = static_cast<QWidget*>(checkbox);
    } else if (aIndex.column() == ID_COLUMN){
        emitErrorOnTableDelegate("El ID no se puede cambiar.");
        return nullptr;
    } else {
        QLineEdit* lineEdit = new QLineEdit(parent);
        lineEdit->setAlignment(Qt::AlignCenter);
        editor = static_cast<QWidget*>(lineEdit);
    }
    emitClearStatusBar();
    return editor;
}

void
MyDelegate::setEditorData(QWidget* aEditor, const QModelIndex& aIndex) const
{
    QString value;

    switch (aIndex.column()) {
    case DNI_COLUMN:
    {
        value = aIndex.model()->data(aIndex, Qt::EditRole).toString();
        emitStoreOldUser(value);
        break;
    }
    case ID_COLUMN:
    {
        emitErrorOnTableDelegate("El ID no se puede cambiar.");
        return;
    }
    case NAME_COLUMN:
    {
        value = aIndex.model()->data(aIndex, Qt::EditRole).toString();
        break;
    }
    case ADMIN_COLUMN:
    {
        bool value = aIndex.model()->data(aIndex, Qt::EditRole).toString() == "X";
        QCheckBox* checkBox = static_cast<QCheckBox*>(aEditor);
        checkBox->setChecked(value);
        return;
    }
    }

    QLineEdit* lineEdit = static_cast<QLineEdit*>(aEditor);
    lineEdit->setText(value);
}

void
MyDelegate::setModelData(QWidget* aEditor, QAbstractItemModel* aModel, const QModelIndex& aIndex) const
{
    if (aIndex.column() == ADMIN_COLUMN) {
        QCheckBox* checkBox = static_cast<QCheckBox*>(aEditor);
        if (checkBox->isChecked()) {
            aModel->setData(aIndex, "X", Qt::EditRole);
        } else {
            aModel->setData(aIndex, "", Qt::EditRole);
        }
    } else {
        QLineEdit* lineEdit = static_cast<QLineEdit*>(aEditor);
        aModel->setData(aIndex, lineEdit->text(), Qt::EditRole);

        if (aIndex.column() == DNI_COLUMN) {
            emitUpdatePasswordForUser(lineEdit->text());
        }
    }
}

void
MyDelegate::updateEditorGeometry(QWidget* aEditor, const QStyleOptionViewItem& aOption, const QModelIndex& aIndex) const
{
    if (aIndex.column() == DNI_COLUMN) {
        QLineEdit* lineEdit = static_cast<QLineEdit*>(aEditor);
        emitUpdatePasswordForUser(lineEdit->text());
    }

    aEditor->setGeometry(aOption.rect);
}

//-----------------------------------------------------------------------------------------------------------------------------

DatosEmpleadosOptions::DatosEmpleadosOptions(NotificationSender* aNotificationSender, QWidget *parent) :
    QWidget(parent),
    mNotificationSender(aNotificationSender),
    mUi(new Ui::DatosEmpleadosOptions)
{
    mUi->setupUi(this);

    mUi->label_Error->setStyleSheet("color: rgba(255,0,0,0.5)");

    mSqlTableModel = new QSqlTableModel();
    mSqlTableModel->setTable(TableNames::OPERARIOS);
    mSqlTableModel->select();
    mUi->tableView->setModel(mSqlTableModel);
    mUi->tableView->horizontalHeader()->setResizeContentsPrecision(QHeaderView::Stretch);
    mUi->tableView->resizeColumnsToContents();

    mDelegate = new MyDelegate(this);
    mUi->tableView->setItemDelegate(mDelegate);

    connect(mDelegate, SIGNAL(errorOnTableDelegate(QString)), this, SLOT(on_errorOnTableDelegate(QString)), Qt::DirectConnection);
    connect(mDelegate, SIGNAL(updatePasswordForUser(QString)), this, SLOT(on_updatePasswordForUser(QString)), Qt::DirectConnection);
    connect(mDelegate, SIGNAL(clearStatusBar()), this, SLOT(on_emitClearStatusBar()), Qt::DirectConnection);
    connect(mDelegate, SIGNAL(storeOldUser(QString)), this, SLOT(on_storeOldUser(QString)), Qt::DirectConnection);
}

DatosEmpleadosOptions::~DatosEmpleadosOptions()
{
    delete mUi;
}

void
DatosEmpleadosOptions::clear()
{
    mUi->lineEdit_DNI->clear();
    mUi->lineEdit_NombreYApellido->clear();

    mUi->tableView->update();
}

void
DatosEmpleadosOptions::setAgregarEmpleadoLayout()
{
    mUi->lineEdit_DNI->clear();
    mUi->lineEdit_NombreYApellido->clear();
    mUi->label_Title->setText("Agregar Empleado");
    mUi->label_Instrucciones->setText("1 - Inserte DNI del nuevo empleado.\n2 - Inserte Nombre y Apellido del nuevo empleado.\n3 - Presione el botón '+'.\n4 - Confirme el resultado de la operación en la barra de estado.");
    mUi->toolButton_Nuevo->show();
    mUi->tableView->hide();
}

void
DatosEmpleadosOptions::setModificarEmpleadoLayout()
{
    mUi->lineEdit_DNI->clear();
    mUi->lineEdit_NombreYApellido->clear();
    mUi->label_Title->setText("Modificar Datos Empleado");
    mUi->label_Instrucciones->setText("1 - Inserte DNI o Nombre del empleado existente.\n2 - Edite los campos sobre la tabla.");
    mUi->toolButton_Nuevo->hide();
    mUi->tableView->show();
    static_cast<QSqlTableModel*>(mUi->tableView->model())->select();
}

void DatosEmpleadosOptions::on_lineEdit_DNI_textChanged(const QString &arg1)
{
    mUi->label_Error->clear();
    if (mUi->tableView->isHidden() == false) {
        QString filter;
        filter.append(OperariosFields::DNI).append(" LIKE '%").append(arg1 + "%'");
        static_cast<QSqlTableModel*>(mUi->tableView->model())->setFilter(filter);
    }
}

void DatosEmpleadosOptions::on_lineEdit_NombreYApellido_textChanged(const QString &arg1)
{
    mUi->label_Error->clear();
    if (mUi->tableView->isHidden() == false) {
        QString filter;
        filter.append(OperariosFields::NOMBRE_COMPLETO).append(" LIKE '%").append(arg1 + "%'");
        static_cast<QSqlTableModel*>(mUi->tableView->model())->setFilter(filter);
    }
}

void DatosEmpleadosOptions::on_toolButton_Nuevo_pressed()
{
    QString dni(mUi->lineEdit_DNI->text());
    if (dni.isEmpty()) {
        mNotificationSender->emitShowError("Inserte DNI.");
        mUi->label_Error->setText("Inserte DNI.");
        return;
    }
    if (exists(TableNames::OPERARIOS, KeyAndValue(OperariosFields::DNI, dni))) {
        mNotificationSender->emitShowError("DNI ya registrado.");
        return;
    }

    QString nombreYApellido(mUi->lineEdit_NombreYApellido->text());
    if (nombreYApellido.isEmpty()) {
        mNotificationSender->emitShowError("Inserte Nombre Y Apellido.");
        mUi->label_Error->setText("Inserte Nombre Y Apellido.");
        return;
    }
    Result<void> res = insert(TableNames::OPERARIOS, {KeyAndValue(OperariosFields::DNI, dni),
                                                      KeyAndValue(OperariosFields::NOMBRE_COMPLETO, nombreYApellido)});
    if (res.status() != Status::SUCCEEDED) {
        mNotificationSender->emitShowError(res.error());
    } else {
        mNotificationSender->emitNewUserAdded(dni);
        mNotificationSender->emitShowInfo("Operario " + nombreYApellido + " correctamente guardado!");
    }
}

void
DatosEmpleadosOptions::on_errorOnTableDelegate(const QString& aError) const
{
    mNotificationSender->showError(aError);
}

void
DatosEmpleadosOptions::on_updatePasswordForUser(const QString& aDNI)
{
    DataBaseUtils::update(TableNames::OPERARIOS, {KeyAndValue(OperariosFields::DNI, aDNI)},{});
    mNotificationSender->emitPasswordNeedsUpdate(mOldDNI, aDNI);
    mOldDNI = QString();
}

void
DatosEmpleadosOptions::on_storeOldUser(const QString& aDNI)
{
    if (mOldDNI.isEmpty()) {
        mOldDNI = aDNI;
        qDebug() << "Store DNI: " + aDNI;
    }
}

void
DatosEmpleadosOptions::on_emitClearStatusBar() const
{
    mNotificationSender->emitClearStatusBar();
}
