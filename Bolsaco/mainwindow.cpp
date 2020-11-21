#include "mainwindow.h"

#include <QFile>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTextStream>

#include <iostream> // USED TO DEV. SHOULD BE GONE

#include "CargaTareas.h"
#include "DatabaseUtils.h"
#include "InicioSesion.h"
#include "NotificationSender.h"
#include "SimpleCrypt.h"

#include "ui_mainwindow.h"

using namespace DataBaseUtils;

MainWindow::MainWindow(QWidget *aParent)
    : QMainWindow(aParent)
    , mCrypt(new SimpleCrypt(Q_UINT64_C(0x0c2ad4a4acb9f023)))
    , mUi(new Ui::MainWindow)
{
    mUi->setupUi(this);

    mCargaTareas = new CargaTareas(mUi->showWidget);
    mCargaTareas->hide();

    mInicioSesion = new InicioSesion(*mCrypt, mUi->showWidget);

    setupDataBase();

    // Carga de usuario y clave. Sacar. Esto tiene que ser un proceso soportado.
    QFile file(QCoreApplication::applicationDirPath() + "/Access.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        QString encripted(mCrypt->encryptToString(QString("34990189,35990189\n")));
        out << encripted;
        file.close();
    }

    connect(mInicioSesion, SIGNAL(successfulLogin()), this, SLOT(on_successfulLogin()));
}

MainWindow::~MainWindow()
{
    if (false) {
        // USED FOR DEV. SHOULD GO AWAY
        mDb.removeDatabase(mDb.connectionName());
        if(QFile(QCoreApplication::applicationDirPath() + "/Bolsaco").remove()) std::cout << "BORRO DB" << std::endl;
        if(QFile(QCoreApplication::applicationDirPath() + "/Access.txt").remove()) std::cout << "BORRO Access" << std::endl;
    } else {
        mDb.close();
    }
    delete mUi;
}

void
MainWindow::setupDataBase()
{
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER) == false) {
        ERROR("SQL Driver is not available");
        throw;
    }

    mDb = QSqlDatabase::addDatabase(DRIVER);
    mDb.setDatabaseName(QCoreApplication::applicationDirPath() + "/Bolsaco");
    if(mDb.open() == false) {
        ERROR(mDb.lastError().text().toStdString());
    }

    // Check database health. Tables that should exist:

    QStringList tables = mDb.tables();
    if (tables.contains(TableNames::BOBINAS) == false) {
        QSqlQuery query(CreationCommands::createBobinas);
        if(query.isActive() == false) {
            ERROR(query.lastError().text().toStdString());
            throw;
        }
    }

    /*if (tables.contains(TableNames::MAQUINA) == false) {
        QSqlQuery query(CreationCommands::createMaquina);
        if(query.isActive() == false) {
            ERROR(query.lastError().text().toStdString());
            throw;
        }

        std::vector<KeyAndValue> maquina;
        maquina.emplace_back(KeyAndValue(MaquinaFields::TIPO, "Extrusora"));
        DataBaseUtils::insert(TableNames::MAQUINA, maquina);
        maquina.clear();
        maquina.emplace_back(KeyAndValue(MaquinaFields::TIPO, "Cortadora"));
        DataBaseUtils::insert(TableNames::MAQUINA, maquina);
        maquina.clear();
        maquina.emplace_back(KeyAndValue(MaquinaFields::TIPO, "Filtradora"));
        DataBaseUtils::insert(TableNames::MAQUINA, maquina);
        maquina.clear();
        maquina.emplace_back(KeyAndValue(MaquinaFields::TIPO, "Lavadora"));
        DataBaseUtils::insert(TableNames::MAQUINA, maquina);
        maquina.clear();
        maquina.emplace_back(KeyAndValue(MaquinaFields::TIPO, "Rebobinadora"));
        DataBaseUtils::insert(TableNames::MAQUINA, maquina);
        maquina.clear();
    }

    if (tables.contains(TableNames::OPERADOR) == false) {
        QSqlQuery query(CreationCommands::createOperador);
        if(query.isActive() == false) {
            ERROR(query.lastError().text().toStdString());
            throw;
        }

        std::vector<KeyAndValue> operador;
        operador.emplace_back(KeyAndValue(OperadorFields::DNI, "34990189"));
        operador.emplace_back(KeyAndValue(OperadorFields::APELLIDO, "Cipo"));
        operador.emplace_back(KeyAndValue(OperadorFields::NOMBRE, "Mauro"));
        DataBaseUtils::insert(TableNames::OPERADOR, operador);
        operador.clear();
        operador.emplace_back(KeyAndValue(OperadorFields::DNI, "35915762"));
        operador.emplace_back(KeyAndValue(OperadorFields::APELLIDO, "Vacotto"));
        operador.emplace_back(KeyAndValue(OperadorFields::NOMBRE, "Meli"));
        DataBaseUtils::insert(TableNames::OPERADOR, operador);
        operador.clear();
    }

    if (tables.contains(TableNames::TAREA) == false) {
        QSqlQuery query(CreationCommands::createTarea);
        if(query.isActive() == false) {
            ERROR(query.lastError().text().toStdString());
            throw;
        }
    }*/

}

void
MainWindow::on_successfulLogin()
{
    mCargaTareas->setCurrentUser(mInicioSesion->getCurrentUser());

    mInicioSesion->hide();
    mCargaTareas->show();
}
