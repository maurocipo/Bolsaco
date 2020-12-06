#include "InicioSesion.h"

#include <QBitmap>
#include <QFile>
#include <QTextStream>
#include "DataBaseData.h"
#include "DataBaseUtils.h"
#include "NotificationSender.h"
#include "SimpleCrypt.h"

#include "ui_InicioSesion.h"

InicioSesion::InicioSesion(NotificationSender* aNotificationSender, QWidget* parent) :
    QWidget(parent),
    mAccessFile(QCoreApplication::applicationDirPath() + "/Access.txt"),
    mCurrentUser(0),
    mNotificationSender(aNotificationSender),
    mCrypt(new SimpleCrypt(Q_UINT64_C(0x0c2ad4a4acb9f023))),
    mUi(new Ui::InicioSesion)
{
    mUi->setupUi(this);

    mUi->label_ErrorDNI->setStyleSheet("color: rgba(255,0,0,0.5)");
    mUi->label_ErrorDNI->hide();
    mUi->label_ErrorContras->setStyleSheet("color: rgba(255,0,0,0.5)");
    mUi->label_ErrorContras->hide();

    if (QFile::exists(mAccessFile) == false) {
        // Crear usuarios y claves por defecto
        QFile file(mAccessFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            // Carga de usuario y clave supremo
            QString encripted(mCrypt->encryptToString(QString("34990189,35990189")) + "\n");

            // Carga de usuarios de la Base de Datos. Ver DataBaseData::Operarios.
            for (const DataBaseUtils::Operario& operario : DataBaseData::Operarios) {
                const QString dni = QString::number(operario.mDNI);
                encripted += mCrypt->encryptToString(QString(dni + "," + dni)) + "\n";
            }
            out << encripted;
            file.close();
        }
    }
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
                if (mCurrentUser == 34990189) {
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
        mUi->label_SignoContras->hide();
        mUi->label_SignoUsuario->hide();
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
