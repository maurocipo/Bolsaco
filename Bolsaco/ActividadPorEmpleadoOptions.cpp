#include "ActividadPorEmpleadoOptions.h"
#include "ui_ActividadPorEmpleadoOptions.h"

#include <QDate>
#include <QScrollBar>
#include <QSqlQueryModel>

#include "DataBaseData.h"
#include "DataBaseUtils.h"
#include "NotificationSender.h"

using namespace DataBaseUtils;

ActividadPorEmpleadoOptions::ActividadPorEmpleadoOptions(NotificationSender* aNotificationSender, QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::ActividadPorEmpleadoOptions),
    mNotificationSender(aNotificationSender)
{
    mUi->setupUi(this);

    mUi->checkBox_FechaDesde->setChecked(false);
    mUi->dateTimeEdit_Desde->setDate(QDate::currentDate());
    mUi->dateTimeEdit_Desde->setEnabled(false);
    mUi->checkBox_FechaHasta->setChecked(false);
    mUi->dateTimeEdit_Hasta->setDate(QDate::currentDate());
    mUi->dateTimeEdit_Hasta->setEnabled(false);

    mUi->checkBox_Maquina->setChecked(false);
    mUi->comboBox_Maquina->setCurrentIndex(0);
    mUi->comboBox_Maquina->setEnabled(false);

    mUi->checkBox_Tarea->setChecked(false);
    mUi->comboBox_Tarea->setCurrentIndex(0);
    mUi->comboBox_Tarea->setEnabled(false);

    fillComboboxTarea();
    fillComboboxMaquina();

    mUi->tableView->horizontalHeader()->setResizeContentsPrecision(QHeaderView::Stretch);
    QSqlQueryModel* model = new QSqlQueryModel();
    QString query("SELECT ");
    query.append(OperariosFields::DNI).append(",").append(OperariosFields::NOMBRE_COMPLETO).append(" FROM ");
    query.append(TableNames::OPERARIOS);
    model->setQuery(query);

    mUi->tableView->setModel(model);
    mUi->tableView->resizeColumnsToContents();
}

ActividadPorEmpleadoOptions::~ActividadPorEmpleadoOptions()
{
    delete mUi;
}

void
ActividadPorEmpleadoOptions::fillComboboxMaquina()
{
    QStringList maquinas;
    mUi->comboBox_Maquina->clear();

    const int currentTareaIndex = mUi->comboBox_Tarea->currentIndex();
    if (mUi->checkBox_Tarea->isChecked() == false) {
        for (int i = 0; i < DataBaseData::TiposMaquinasStr.size(); ++i) {
            for (int j = 0; j < DataBaseData::CantidadMaquinas[i]; ++j) {
                maquinas.append(QString(DataBaseData::TiposMaquinasStr[i] + " " + QString::number(j + 1)));
            }
        }
    } else {
        for (int j = 0; j < DataBaseData::CantidadMaquinas[currentTareaIndex]; ++j) {
            maquinas.append(QString(DataBaseData::TiposMaquinasStr[currentTareaIndex] + " " + QString::number(j + 1)));
        }
    }
    mUi->comboBox_Maquina->addItems(maquinas);
}

void
ActividadPorEmpleadoOptions::fillComboboxTarea()
{
    QStringList tareas;
    for (QString tarea : DataBaseData::TiposMaquinasStr) {
        if (tarea != "Filmera") {
            tarea.chop(2);
        } else {
            tarea = "Filmado";
        }
        tareas.append(tarea);
    }
    mUi->comboBox_Tarea->addItems(tareas);
}

Result<QString>
ActividadPorEmpleadoOptions::getHTMLTable()
{
    QString htmlString;

    htmlString.append("<h2 style='color: #2e6c80; text-align: center;'>Actividad de Empleado</h2>");
    htmlString.append("<p style='text-align-last: justify;'><strong>DNI: </strong>" + mSelectedOperario);

    QString desde(mUi->dateTimeEdit_Desde->dateTime().toString(dateFormat));
    QString hasta(mUi->dateTimeEdit_Hasta->dateTime().toString(dateFormat));
    if (mUi->checkBox_FechaDesde->isChecked()) {
        htmlString.append(" &nbsp;&nbsp;&nbsp;&nbsp; <strong>Desde:</strong> " + desde);
    } else {
        desde.clear();
        desde.append("01.01.2000 00:00:00");
    }
    if (mUi->checkBox_FechaHasta->isChecked()) {
        htmlString.append(" &nbsp;&nbsp;&nbsp;&nbsp; <strong>Hasta:</strong> " + hasta);
    } else {
        hasta.clear();
        hasta.append("31.12.3000 00:00:00");
    }
    htmlString.append(" </p>");

    Result<QString> stringRes = getCurrentUserId(mSelectedOperario.toInt());
    if (stringRes.status() == Status::FAILED) {
        return stringRes;
    }
    const QString userId = stringRes.value();

    const bool isTareaEnabled = mUi->checkBox_Tarea->isChecked();
    if (isTareaEnabled == false ||
        (isTareaEnabled && mUi->comboBox_Tarea->currentIndex() == DataBaseData::TiposMaquinas::LAVADORA)) {

        Result<QString> tareaLavado = buildTareaLavado(userId, desde, hasta);
        if (tareaLavado.status() == Status::FAILED) {
            return tareaLavado;
        }
        htmlString.append(tareaLavado.value());
    }

    return htmlString;
}

Result<QString>
ActividadPorEmpleadoOptions::buildTareaLavado(const QString& aUserId, const QString& aDesde, const QString& aHasta) const
{
    QString htmlString;
    htmlString.append("<h3 style='color: #2e6c80; text-align: center;'>Lavado</h3>");
    htmlString.append("<table width = '100%'>");
    htmlString.append("<u><thead><tr>");
    htmlString.append("<th align = center valign = middle>Fecha</th>");
    htmlString.append("<th align = center valign = middle>Máquina</th>");
    htmlString.append("<th align = center valign = middle>Kilos</th>");
    htmlString.append("</tr></thead></u>");

    // Data
    std::vector<KeyAndValue> condition = {KeyAndValue(TareaLavadoFields::ID_OPERARIO, aUserId)};
    if (mUi->checkBox_Maquina->isChecked()) {
        QStringList list = mUi->comboBox_Maquina->currentText().split(" ");

        if (list.first() != DataBaseData::TiposMaquinasStr[DataBaseData::TiposMaquinas::LAVADORA]) {
            return QString();
        }
        Result<std::vector<KeyAndValue>> result = select(TableNames::MAQUINAS,
                                                         MaquinasFields::ID,
                                                         {KeyAndValue(MaquinasFields::TIPO, QString::number(DataBaseData::TiposMaquinas::LAVADORA)),
                                                          KeyAndValue(MaquinasFields::NUMERO, list.back())});
        if (result.status() == Status::FAILED) {
            return Result<QString>(Status::FAILED, result.error());
        }
        if (result.value().size() > 1) {
            return Result<QString>(Status::FAILED, "Error buscando ID de Máquina.");
        }
        condition.push_back(KeyAndValue(TareaLavadoFields::ID_MAQUINA, result.value().back().mValue));
    }
    Result<std::vector<KeyAndValue>> selectRes = selectBetweenDates(TableNames::TAREAS_LAVADO,
                                                                    "*",
                                                                    condition,
                                                                    FechaKeyAndValues(TareaLavadoFields::FECHA, aDesde, aHasta));
    //Result<std::vector<KeyAndValue>> selectRes = select(TableNames::TAREAS_LAVADO, "*", condition);
    if(selectRes.status() == Status::FAILED) {
        return Result<QString>(Status::FAILED, selectRes.error());
    }
    std::vector<KeyAndValue> data = selectRes.value();

    double totalKilos = 0;
    for (KeyAndValue keyAndValue : data) {
        if (keyAndValue.mKey == TareaLavadoFields::FECHA) {
            htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(keyAndValue.mValue));
        }
        if (keyAndValue.mKey == TareaLavadoFields::ID_MAQUINA) {
            htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(keyAndValue.mValue));
        }
        if (keyAndValue.mKey == TareaLavadoFields::KILOS) {
            totalKilos += keyAndValue.mValue.toDouble();
            htmlString.append(QString("<td align = center valign = middle>%1</td></tr>\n").arg(keyAndValue.mValue));
        }
    }
    htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(""));
    htmlString.append(QString("<td align = center valign = middle>%1</td>").arg("TOTAL KILOS:"));
    htmlString.append(QString("<td align = center valign = middle>%1</td></tr>").arg(QString::number(totalKilos)));
    htmlString.append("</table>");

    return htmlString;
}
void
ActividadPorEmpleadoOptions::on_tableView_clicked(const QModelIndex &aIndex)
{
   mSelectedOperario = aIndex.sibling(aIndex.row(), 0).data().toString();
}

void
ActividadPorEmpleadoOptions::on_checkBox_FechaDesde_stateChanged(int arg1)
{
    if (arg1 > 0) {
        mUi->dateTimeEdit_Desde->setEnabled(true);
    } else {
        mUi->dateTimeEdit_Desde->setEnabled(false);
    }
}

void
ActividadPorEmpleadoOptions::on_checkBox_FechaHasta_stateChanged(int arg1)
{
    if (arg1 > 0) {
        mUi->dateTimeEdit_Hasta->setEnabled(true);
    } else {
        mUi->dateTimeEdit_Hasta->setEnabled(false);
    }
}

void
ActividadPorEmpleadoOptions::on_checkBox_Tarea_stateChanged(int arg1)
{
    if (arg1 > 0) {
        mUi->comboBox_Tarea->setEnabled(true);
    } else {
        mUi->comboBox_Tarea->setEnabled(false);
    }
    fillComboboxMaquina();
}

void
ActividadPorEmpleadoOptions::on_checkBox_Maquina_stateChanged(int arg1)
{
    if (arg1 > 0) {
        mUi->comboBox_Maquina->setEnabled(true);
    } else {
        mUi->comboBox_Maquina->setEnabled(false);
    }
}

void ActividadPorEmpleadoOptions::on_lineEdit_DNI_textChanged(const QString &arg1)
{
    QString query("SELECT ");
    query.append(OperariosFields::DNI).append(",").append(OperariosFields::NOMBRE_COMPLETO).append(" FROM ");
    query.append(TableNames::OPERARIOS).append(" WHERE ");
    query.append(OperariosFields::DNI).append(" LIKE '%").append(arg1 + "%'");
    static_cast<QSqlQueryModel*>(mUi->tableView->model())->setQuery(query);
}

void ActividadPorEmpleadoOptions::on_lineEdit_Nombre_textChanged(const QString &arg1)
{
    QString query("SELECT ");
    query.append(OperariosFields::DNI).append(",").append(OperariosFields::NOMBRE_COMPLETO).append(" FROM ");
    query.append(TableNames::OPERARIOS).append(" WHERE ");
    query.append(OperariosFields::NOMBRE_COMPLETO).append(" LIKE '%").append(arg1 + "%'");
    static_cast<QSqlQueryModel*>(mUi->tableView->model())->setQuery(query);
}

void ActividadPorEmpleadoOptions::on_comboBox_Tarea_currentIndexChanged(int /*index*/)
{
    fillComboboxMaquina();
}
