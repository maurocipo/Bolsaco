#include "mainwindow.h"

#include <QFile>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTextStream>

#include <iostream> // USED TO DEV. SHOULD BE GONE

#include "CargaTareas.h"
#include "DataBaseUtils.h"
#include "DataBaseData.h"
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
    // DataBaseUtils::TableNames::BOBINAS
    // DataBaseUtils::TableNames::MAQUINAS
    // DataBaseUtils::TableNames::MEDIDAS_BOBINAS
    // DataBaseUtils::TableNames::MEDIDAS_BOLSAS
    // DataBaseUtils::TableNames::OPERARIOS
    // DataBaseUtils::TableNames::TAREAS_CORTADO
    // DataBaseUtils::TableNames::TAREAS_EXTRUSADO
    // DataBaseUtils::TableNames::TAREAS_FILTRADO
    // DataBaseUtils::TableNames::TAREAS_LAVADO
    // DataBaseUtils::TableNames::TAREAS_REBOBINADO
    // DataBaseUtils::TableNames::TIPOS_MAQUINAS
    QStringList tables = mDb.tables();
    if (tables.contains(TableNames::BOBINAS) == false) {
        QSqlQuery query(CreationCommands::createBobinas);
        if(query.isActive() == false) {
            ERROR(query.lastError().text().toStdString());
            throw;
        }
    }

    if (tables.contains(TableNames::MAQUINAS) == false) {
        QSqlQuery query(CreationCommands::createMaquinas);
        if(query.isActive() == false) {
            ERROR(query.lastError().text().toStdString());
            throw;
        }

        for (size_t i = 0; i < DataBaseData::Maquinas.size(); ++i) {
            std::vector<KeyAndValue> maquina;
            maquina.emplace_back(KeyAndValue(MaquinasFields::DESCRIPCION, DataBaseData::Maquinas[i].mDescripcion));
            maquina.emplace_back(KeyAndValue(MaquinasFields::TIPO, QString(DataBaseData::Maquinas[i].mTipo)));
            DataBaseUtils::insert(TableNames::MAQUINAS, maquina);
        }
    }

    if (tables.contains(TableNames::MEDIDAS_BOBINAS) == false) {
        QSqlQuery query(CreationCommands::createMedidasBobinas);
        if(query.isActive() == false) {
            ERROR(query.lastError().text().toStdString());
            throw;
        }

        for (int i = 0; i < DataBaseData::MedidasBobinas.size(); ++i) {
            KeyAndValue medida(MedidasBobinasFields::DESCRIPCION, DataBaseData::MedidasBobinas[i]);
            DataBaseUtils::insert(TableNames::MEDIDAS_BOBINAS, medida);
        }
    }

    if (tables.contains(TableNames::MEDIDAS_BOLSAS) == false) {
        QSqlQuery query(CreationCommands::createMedidasBolsas);
        if(query.isActive() == false) {
            ERROR(query.lastError().text().toStdString());
            throw;
        }

        for (int i = 0; i < DataBaseData::MedidasBolsas.size(); ++i) {
            KeyAndValue medida(MedidasBobinasFields::DESCRIPCION, DataBaseData::MedidasBolsas[i]);
            DataBaseUtils::insert(TableNames::MEDIDAS_BOLSAS, medida);
        }
    }

    if (tables.contains(TableNames::OPERARIOS) == false) {
        QSqlQuery query(CreationCommands::createOperarios);
        if(query.isActive() == false) {
            ERROR(query.lastError().text().toStdString());
            throw;
        }

        for (size_t i = 0; i < DataBaseData::Operarios.size(); ++i) {
            std::vector<KeyAndValue> operario;
            operario.emplace_back(OperariosFields::DNI, QString(DataBaseData::Operarios[i].mDNI));
            operario.emplace_back(OperariosFields::NOMBRE_COMPLETO, QString(DataBaseData::Operarios[i].mNombreYApellido));
            DataBaseUtils::insert(TableNames::OPERARIOS, operario);
        }
    }

    if (tables.contains(TableNames::TAREAS_CORTADO) == false) {
        QSqlQuery query(CreationCommands::createTareasCortado);
        if(query.isActive() == false) {
            ERROR(query.lastError().text().toStdString());
            throw;
        }
    }

    if (tables.contains(TableNames::TAREAS_EXTRUSADO) == false) {
        QSqlQuery query(CreationCommands::createTareasExtrusado);
        if(query.isActive() == false) {
            ERROR(query.lastError().text().toStdString());
            throw;
        }
    }

    if (tables.contains(TableNames::TAREAS_FILTRADO) == false) {
        QSqlQuery query(CreationCommands::createTareasFiltrado);
        if(query.isActive() == false) {
            ERROR(query.lastError().text().toStdString());
            throw;
        }
    }

    if (tables.contains(TableNames::TAREAS_LAVADO) == false) {
        QSqlQuery query(CreationCommands::createTareasLavado);
        if(query.isActive() == false) {
            ERROR(query.lastError().text().toStdString());
            throw;
        }
    }

    if (tables.contains(TableNames::TAREAS_REBOBINADO) == false) {
        QSqlQuery query(CreationCommands::createTareasRebobinado);
        if(query.isActive() == false) {
            ERROR(query.lastError().text().toStdString());
            throw;
        }
    }

    if (tables.contains(TableNames::TIPOS_MAQUINAS) == false) {
        QSqlQuery query(CreationCommands::createTiposMaquinas);
        if(query.isActive() == false) {
            ERROR(query.lastError().text().toStdString());
            throw;
        }

        for (int i = 0; i < DataBaseData::TiposMaquinasStr.size(); ++i) {
            DataBaseUtils::insert(TableNames::TIPOS_MAQUINAS, KeyAndValue(TiposMaquinasFields::TIPO, DataBaseData::TiposMaquinasStr[i]));
        }
    }

}

void
MainWindow::on_successfulLogin()
{
    mCargaTareas->setCurrentUser(mInicioSesion->getCurrentUser());

    mInicioSesion->hide();
    mCargaTareas->show();
}
