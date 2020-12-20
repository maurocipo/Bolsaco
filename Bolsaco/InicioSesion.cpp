#include "InicioSesion.h"

#include <QBitmap>
#include <QFile>
#include <QRegExpValidator>
#include <QShowEvent>
#include <QTextStream>
#include "DataBaseData.h"
#include "DataBaseUtils.h"
#include "NotificationSender.h"
#include "SimpleCrypt.h"

#include "ui_InicioSesion.h"

InicioSesion::InicioSesion(NotificationSender* aNotificationSender, QWidget* parent) :
    QWidget(parent),
    mAccessFile(QCoreApplication::applicationDirPath() + "/data1"),
    mCurrentUser(0),
    mNotificationSender(aNotificationSender),
    mCrypt(new SimpleCrypt(Q_UINT64_C(0x0c2ad4a4acb9f023))),
    mUi(new Ui::InicioSesion)
{
    mUi->setupUi(this);

    mUi->lineEdit_DNI->setValidator(new QRegExpValidator(QRegExp("[0-9]*"), mUi->lineEdit_DNI));

    mUi->label_ErrorDNI->setStyleSheet("color: rgba(255,0,0,0.5)");
    mUi->label_ErrorDNI->hide();
    mUi->label_ErrorContras->setStyleSheet("color: rgba(255,0,0,0.5)");
    mUi->label_ErrorContras->hide();

    if (QFile::exists(mAccessFile) == false) {
        // Crear usuarios y claves por defecto
        QFile file(mAccessFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            QString encripted;

            // Carga de usuarios de la Base de Datos. Ver DataBaseData::Operarios.
            // La clave por defecto de los usuarios Admin es su DNI + 1.000.000.
            for (const DataBaseUtils::Operario& operario : DataBaseData::Operarios) {
                const QString dni = QString::number(operario.mDNI);
                if (operario.mIsAdmin) {
                    encripted += mCrypt->encryptToString(QString(dni + "," + QString::number(dni.toInt() + 1000000))) + "\n";
                } else {
                    encripted += mCrypt->encryptToString(QString(dni + "," + dni)) + "\n";
                }
            }
            out << encripted;
            file.close();
        }
    }

    connect(mNotificationSender, SIGNAL(newUserAdded(QString)), this, SLOT(on_newUserAdded(const QString&)));
    connect(mNotificationSender, SIGNAL(passwordNeedsUpdate(QString, QString)), this, SLOT(on_passwordNeedsUpdate(const QString&, const QString&)));
}

InicioSesion::~InicioSesion()
{
    delete mUi;
}

void InicioSesion::on_pushButton_IniciarSesion_pressed()
{
    if (mUi->label_ErrorDNI->isVisible()) {
        mNotificationSender->emitShowError("El DNI no esta regstrado!");
        return;
    }

    if (QFile::exists(mAccessFile) == false) {
        mNotificationSender->emitShowError("No existe el archivo de claves!");
        return;
    }

    // Get user and password introduced by user.
    const QString DNI = mUi->lineEdit_DNI->text();
    const QString password = mUi->lineEdit_Contrasena->text();

    bool successfulLogin = false;
    // Read the file and try to find the user.
    QFile file(mAccessFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString read(in.readLine());
            QString userData = mCrypt->decryptToString(read);
            QStringList splittedData = userData.split(',');
            if (splittedData[0] == DNI && splittedData[1] == password) {
                mCurrentUser = DNI.toInt();

                Result<bool> result = DataBaseUtils::isAdmin(DNI);
                if (result.status() != Status::SUCCEEDED) {
                    mNotificationSender->emitShowError(result.error());
                    return;
                }
                if (result.value()) {
                    emitAdministratorLogin();
                    mNotificationSender->emitShowWarning(mNotificationSender->WARN_ADMIN_USER_LOGGED);
                } else {
                    mNotificationSender->emitShowInfo("Login exitoso!");
                    emitOperarioLogin();
                }
                successfulLogin = true;
                break;
            }
        }
    }
    file.close();

    if (successfulLogin) {
        mUi->label_ErrorContras->hide();
        mUi->label_SignoContras->setStyleSheet("border-image: 0 0 0 0 stretch stretch;");
        mUi->label_SignoUsuario->setStyleSheet("border-image: 0 0 0 0 stretch stretch;");
        mUi->lineEdit_Contrasena->clear();
        mUi->lineEdit_DNI->clear();
    } else {
        mUi->label_SignoContras->setStyleSheet("border-image: url(:Images/Incorrect.png) 0 0 0 0 stretch stretch;");
        mUi->label_SignoContras->show();
        mUi->label_ErrorContras->show();
        mNotificationSender->emitShowError("Contraseña Incorrecta.");
    }
}

void
InicioSesion::emitOperarioLogin()
{
    emit operarioLogin();
}

void
InicioSesion::emitAdministratorLogin()
{
    emit administratorLogin();
}

void InicioSesion::on_lineEdit_Contrasena_returnPressed()
{
    on_pushButton_IniciarSesion_pressed();
}

void InicioSesion::on_lineEdit_DNI_editingFinished()
{
    mUi->label_SignoUsuario->setMinimumSize(25, 25);
    if (DataBaseUtils::exists(DataBaseUtils::TableNames::OPERARIOS, DataBaseUtils::KeyAndValue(DataBaseUtils::OperariosFields::DNI, mUi->lineEdit_DNI->text())) == false) {
        mUi->label_SignoUsuario->setStyleSheet("border-image: url(:Images/Incorrect.png) 0 0 0 0 stretch stretch;");
        mUi->label_ErrorDNI->show();
        mNotificationSender->emitShowError("El DNI no esta regstrado!");
    } else {
        mUi->label_SignoUsuario->setStyleSheet("border-image: url(:Images/Correct.png) 0 0 0 0 stretch stretch;");
        mUi->label_ErrorDNI->hide();
        mNotificationSender->clearStatusBar();
    }
    mUi->label_SignoUsuario->show();
}

void InicioSesion::on_newUserAdded(const QString& aDNI)
{
    QString encripted = mCrypt->encryptToString(QString(aDNI + "," + aDNI)) + "\n";
    QFile file(mAccessFile);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream out(&file);
        out << encripted;
        file.close();
    }
}

void
InicioSesion::on_passwordNeedsUpdate(const QString& aOldDNI, const QString& aNewDNI)
{
    QFile file(mAccessFile);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream in(&file);
        QTextStream out(&file);
        while (!in.atEnd()) {
            size_t pos = in.pos();
            QString read(in.readLine());
            QString userData = mCrypt->decryptToString(read);
            QStringList splittedData = userData.split(',');
            if (splittedData[0] == aOldDNI) {
                Result<bool> result = DataBaseUtils::isAdmin(aOldDNI);
                if (result.status() != Status::SUCCEEDED) {
                    mNotificationSender->emitShowError(result.error());
                    return;
                }

                QString encripted;
                if (result.value()) {
                    encripted += mCrypt->encryptToString(QString(aNewDNI + "," + QString::number(aNewDNI.toInt() + 1000000))) + "\n";
                } else {
                    encripted += mCrypt->encryptToString(QString(aNewDNI + "," + aNewDNI)) + "\n";
                }
                out.seek(pos);
                out << encripted;
                break;
            }
        }
    }
    file.close();
}
