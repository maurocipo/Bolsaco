#include "InicioSesion.h"

#include <QFile>
#include <QTextStream>
#include "SimpleCrypt.h"

#include "ui_InicioSesion.h"

InicioSesion::InicioSesion(SimpleCrypt& aCrypt,
                           QWidget* parent) :
    QWidget(parent),
    mCurrentUser(0),
    mCrypt(&aCrypt),
    mUi(new Ui::InicioSesion)
{
    mUi->setupUi(this);
}

InicioSesion::~InicioSesion()
{
    delete mUi;
}

void InicioSesion::on_pushButton_IniciarSesion_pressed()
{
    // Get user and password introduced by user.
    const QString DNI = mUi->lineEdit_DNI->text();
    const QString password = mUi->lineEdit_Contrasena->text();

    // Read the file and try to find the user.
    QFile file(QCoreApplication::applicationDirPath() + "/Access.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString userData = mCrypt->decryptToString(in.readLine());
            userData.chop(1);
            QStringList splittedData = userData.split(',');
            if (splittedData[0] == DNI && splittedData[1] == password) {
                emitSuccessfulLogin();
                break;
            }
        }
        file.close();
    }
}

void
InicioSesion::emitSuccessfulLogin()
{
    emit successfulLogin();
}

void InicioSesion::on_lineEdit_Contrasena_returnPressed()
{
    on_pushButton_IniciarSesion_pressed();
}
