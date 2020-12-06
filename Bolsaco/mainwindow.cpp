#include "mainwindow.h"

#include <QFile>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTextStream>

#include <iostream> // USED TO DEV. SHOULD BE GONE

#include "Admin.h"
#include "CargaTareas.h"
#include "DataBaseUtils.h"
#include "DataBaseData.h"
#include "InicioSesion.h"
#include "NotificationSender.h"

#include "ui_mainwindow.h"

using namespace DataBaseUtils;

MainWindow::MainWindow(QWidget *aParent)
    : QMainWindow(aParent)
    , mUi(new Ui::MainWindow)
{
    mUi->setupUi(this);
    mUi->pushButton_CerrarSesion->hide();

    // Remove resize little arrow down-right corner.
    mUi->statusbar->setSizeGripEnabled(false);

    QGraphicsOpacityEffect* eff = new QGraphicsOpacityEffect(this);
    mUi->statusbar->setGraphicsEffect(eff);

    mStatusBarFadeOutAnimation = new QPropertyAnimation(eff, "opacity");
    connect(mStatusBarFadeOutAnimation, SIGNAL(finished()), this, SLOT(on_statusBarFadeOutFinished()));
    mStatusBarFadeOutAnimation->setDuration(350);
    mStatusBarFadeOutAnimation->setStartValue(1);
    mStatusBarFadeOutAnimation->setEndValue(0);
    mStatusBarFadeOutAnimation->setEasingCurve(QEasingCurve::InQuad);

    mStatusBarFadeInAnimation = new QPropertyAnimation(eff, "opacity");
    connect(mStatusBarFadeOutAnimation, SIGNAL(finished()), this, SLOT(on_statusBarFadeInFinished()));
    mStatusBarFadeInAnimation->setDuration(350);
    mStatusBarFadeInAnimation->setStartValue(0);
    mStatusBarFadeInAnimation->setEndValue(1);
    mStatusBarFadeInAnimation->setEasingCurve(QEasingCurve::OutQuad);


    setupDataBase();

    mNotificationSender = new NotificationSender();

    mAdmin = new Admin(mNotificationSender, mUi->showWidget);
    mAdmin->hide();

    mCargaTareas = new CargaTareas(mNotificationSender, mUi->showWidget);
    mCargaTareas->hide();

    mInicioSesion = new InicioSesion(mNotificationSender, mUi->showWidget);

    connect(mInicioSesion, SIGNAL(operarioLogin()), this, SLOT(on_operarioLogin()));
    connect(mInicioSesion, SIGNAL(administratorLogin()), this, SLOT(on_administratorLogin()));
    connect(mNotificationSender, SIGNAL(showError(QString)), this, SLOT(on_showError(const QString&)));
    connect(mNotificationSender, SIGNAL(showWarning(QString)), this, SLOT(on_showWarning(const QString&)));
    connect(mNotificationSender, SIGNAL(showInfo(QString)), this, SLOT(on_showInfo(const QString&)));
    connect(mNotificationSender, SIGNAL(clearStatusBar()), this, SLOT(on_clearStatusBar()));
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
MainWindow::hideViews()
{
    mAdmin->hide();
    mCargaTareas->hide();
    mInicioSesion->hide();

    mUi->pushButton_CerrarSesion->hide();
    mUi->label_UserName->clear();
}

void
MainWindow::setupDataBase()
{
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER) == false) {
        on_showError("SQL Driver is not available");
        return;
    }

    mDb = QSqlDatabase::addDatabase(DRIVER);
    mDb.setDatabaseName(QCoreApplication::applicationDirPath() + "/Bolsaco");
    if(mDb.open() == false) {
        on_showError(mDb.lastError().text());
        return;
    }

    // Check database health. Tables that should exist:
    // DataBaseUtils::TableNames::BOBINAS
    // DataBaseUtils::TableNames::MAQUINAS
    // DataBaseUtils::TableNames::MEDIDAS_BOBINAS
    // DataBaseUtils::TableNames::MEDIDAS_BOLSAS
    // DataBaseUtils::TableNames::OPERARIOS
    // DataBaseUtils::TableNames::PRODUCTOS_REBOBINADO
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
            on_showError(query.lastError().text());
            return;
        }
    }

    if (tables.contains(TableNames::MAQUINAS) == false) {
        QSqlQuery query(CreationCommands::createMaquinas);
        if(query.isActive() == false) {
            on_showError(query.lastError().text());
            return;
        }

        for (size_t i = 0; i < DataBaseData::Maquinas.size(); ++i) {
            std::vector<KeyAndValue> maquina;
            maquina.emplace_back(KeyAndValue(MaquinasFields::DESCRIPCION, DataBaseData::Maquinas[i].mDescripcion));
            maquina.emplace_back(KeyAndValue(MaquinasFields::TIPO, QString::number(DataBaseData::Maquinas[i].mTipo)));
            maquina.emplace_back(KeyAndValue(MaquinasFields::NUMERO, QString::number(DataBaseData::Maquinas[i].mNumero)));
            Result<void> res = DataBaseUtils::insert(TableNames::MAQUINAS, maquina);
            if (res.status() != Status::SUCCEEDED) {
                on_showError("Error cargando tabla MAQUINAS: " + res.error());
                return;
            }
        }

        if (size_t(DataBaseUtils::getTableEntriesCount(TableNames::MAQUINAS).value()) != DataBaseData::Maquinas.size()) {
            on_showError("Tabla MAQUINAS mal cargada.");
            return;
        }
    }

    if (tables.contains(TableNames::MEDIDAS_BOBINAS) == false) {
        QSqlQuery query(CreationCommands::createMedidasBobinas);
        if(query.isActive() == false) {
            on_showError(query.lastError().text());
            return;
        }

        for (int i = 0; i < DataBaseData::MedidasAnchoBobinas.size(); ++i) {
            for (int j = 0; j < DataBaseData::MedidasMicronajeBobinas.size(); ++j) {
                std::vector<KeyAndValue> medida;
                medida.emplace_back(MedidasBobinasFields::ANCHO, DataBaseData::MedidasAnchoBobinas[i]);
                medida.emplace_back(MedidasBobinasFields::MICRONAJE, DataBaseData::MedidasMicronajeBobinas[j]);
                Result<void> res = DataBaseUtils::insert(TableNames::MEDIDAS_BOBINAS, medida);
                if (res.status() != Status::SUCCEEDED) {
                    on_showError("Error cargando tabla MEDIDAS_BOBINAS: " + res.error());
                    return;
                }
            }
        }

        if(DataBaseUtils::getTableEntriesCount(TableNames::MEDIDAS_BOBINAS).value() !=
           (DataBaseData::MedidasAnchoBobinas.size() * DataBaseData::MedidasMicronajeBobinas.size())) {
            on_showError("Tabla MEDIDAS_BOBINAS mal cargada.");
            return;
        }
    }

    if (tables.contains(TableNames::MEDIDAS_BOLSAS) == false) {
        QSqlQuery query(CreationCommands::createMedidasBolsas);
        if(query.isActive() == false) {
            on_showError(query.lastError().text());
            return;
        }

        for (int i = 0; i < DataBaseData::MedidasLargoBolsas.size(); ++i) {
            KeyAndValue medida(MedidasBolsasFields::LARGO, DataBaseData::MedidasLargoBolsas[i]);
            Result<void> res = DataBaseUtils::insert(TableNames::MEDIDAS_BOLSAS, medida);
            if (res.status() != Status::SUCCEEDED) {
                on_showError("Error cargando tabla MEDIDAS_BOLSAS: " + res.error());
                return;
            }
        }
        if(DataBaseUtils::getTableEntriesCount(TableNames::MEDIDAS_BOLSAS).value() != DataBaseData::MedidasLargoBolsas.size()) {
            on_showError("Tabla MEDIDAS_BOLSAS mal cargada.");
            return;
        }
    }

    if (tables.contains(TableNames::OPERARIOS) == false) {
        QSqlQuery query(CreationCommands::createOperarios);
        if(query.isActive() == false) {
            on_showError(query.lastError().text());
            return;
        }

        for (size_t i = 0; i < DataBaseData::Operarios.size(); ++i) {
            std::vector<KeyAndValue> operario;
            operario.emplace_back(OperariosFields::DNI, QString::number(DataBaseData::Operarios[i].mDNI));
            operario.emplace_back(OperariosFields::NOMBRE_COMPLETO, QString(DataBaseData::Operarios[i].mNombreYApellido));
            Result<void> res = DataBaseUtils::insert(TableNames::OPERARIOS, operario);
            if (res.status() != Status::SUCCEEDED) {
                on_showError("Error cargando tabla OPERARIOS: " + res.error());
                return;
            }
        }
        if(size_t(DataBaseUtils::getTableEntriesCount(TableNames::OPERARIOS).value()) != DataBaseData::Operarios.size()) {
            on_showError("Tabla OPERARIOS mal cargada.");
            return;
        }
    }

    if (tables.contains(TableNames::PRODUCTOS_REBOBINADO) == false) {
        QSqlQuery query(CreationCommands::createProductosRebobinado);
        if(query.isActive() == false) {
            on_showError(query.lastError().text());
            return;
        }
        for (int i = 0; i < DataBaseData::ProductosRebobinadoStr.size(); ++i) {
            Result<void> res = DataBaseUtils::insert(TableNames::PRODUCTOS_REBOBINADO,
                                                     KeyAndValue(ProductosRebobinadoFields::DESCRIPCION, DataBaseData::ProductosRebobinadoStr[i]));
            if (res.status() != Status::SUCCEEDED) {
                on_showError("Error cargando tabla PRODUCTOS_REBOBINADO: " + res.error());
                return;
            }
        }
        if(size_t(DataBaseUtils::getTableEntriesCount(TableNames::PRODUCTOS_REBOBINADO).value()) != size_t(DataBaseData::ProductosRebobinadoStr.size())) {
            on_showError("Tabla PRODUCTOS_REBOBINADO mal cargada.");
            return;
        }
    }

    if (tables.contains(TableNames::TAREAS_CORTADO) == false) {
        QSqlQuery query(CreationCommands::createTareasCortado);
        if(query.isActive() == false) {
            on_showError(query.lastError().text());
            return;
        }
    }

    if (tables.contains(TableNames::TAREAS_EXTRUSADO) == false) {
        QSqlQuery query(CreationCommands::createTareasExtrusado);
        if(query.isActive() == false) {
            on_showError(query.lastError().text());
            return;
        }
    }

    if (tables.contains(TableNames::TAREAS_FILTRADO) == false) {
        QSqlQuery query(CreationCommands::createTareasFiltrado);
        if(query.isActive() == false) {
            on_showError(query.lastError().text());
            return;
        }
    }

    if (tables.contains(TableNames::TAREAS_LAVADO) == false) {
        QSqlQuery query(CreationCommands::createTareasLavado);
        if(query.isActive() == false) {
            on_showError(query.lastError().text());
            return;
        }
    }

    if (tables.contains(TableNames::TAREAS_REBOBINADO) == false) {
        QSqlQuery query(CreationCommands::createTareasRebobinado);
        if(query.isActive() == false) {
            on_showError(query.lastError().text());
            throw;
        }
    }

    if (tables.contains(TableNames::TIPOS_MAQUINAS) == false) {
        QSqlQuery query(CreationCommands::createTiposMaquinas);
        if(query.isActive() == false) {
            on_showError(query.lastError().text());
            return;
        }

        for (int i = 0; i < DataBaseData::TiposMaquinasStr.size(); ++i) {

            Result<void> res = DataBaseUtils::insert(TableNames::TIPOS_MAQUINAS, KeyAndValue(TiposMaquinasFields::TIPO, DataBaseData::TiposMaquinasStr[i]));
            if (res.status() != Status::SUCCEEDED) {
                on_showError("Error cargando tabla TIPOS_MAQUINAS: " + res.error());
                return;
            }
        }
    }

}

void
MainWindow::on_operarioLogin()
{
    if (setUser()) {
        mInicioSesion->hide();
        mCargaTareas->show();
    }
}

void
MainWindow::on_administratorLogin()
{
    if (setUser()) {
        mInicioSesion->hide();
        mAdmin->show();
    }
}

bool
MainWindow::setUser()
{
    const int currentUser = mInicioSesion->getCurrentUser();
    mCargaTareas->setCurrentUser(currentUser);

    // Get user name and set it on the top Label.
    if (DataBaseUtils::exists(TableNames::OPERARIOS,
                              KeyAndValue(OperariosFields::DNI, QString::number(currentUser))) == false) {
        on_showError("El operario no existe en la base de datos.");
        return false;
    }

    std::vector<KeyAndValue> conditions = {KeyAndValue(OperariosFields::DNI, QString::number(currentUser))};
    Result<std::vector<KeyAndValue>> result = DataBaseUtils::select(TableNames::OPERARIOS, OperariosFields::NOMBRE_COMPLETO, conditions);
    if (result.status() != Status::SUCCEEDED) {
        on_showError(result.error());
        return false;
    }
    if (result.value().size() != 1) {
        on_showError("Error buscando Operario. Posible DNI duplicado?");
        return false;
    }
    const QString nombreCompleto = result.value().begin()->mValue;

    mUi->label_UserName->setText(nombreCompleto);
    mUi->pushButton_CerrarSesion->show();

    return true;
}

void
MainWindow::on_pushButton_CerrarSesion_clicked()
{
    hideViews();
    on_clearStatusBar();
    mInicioSesion->show();
}

void
MainWindow::on_showError(const QString& aMessage)
{
    mStatusBarFadeOutAnimation->start();
    mFadeCycles = 10;

    mUi->statusbar->setStyleSheet("background: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 rgba(255,0,0,0.3), stop:1 rgba(240,240,240,255))");
    mUi->statusbar->showMessage(aMessage);
}

void
MainWindow::on_showWarning(const QString& aWarning)
{
    mStatusBarFadeOutAnimation->start();
    mFadeCycles = 1;

    mUi->statusbar->setStyleSheet("background: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 rgba(255,255,0,0.3), stop:1 rgba(240,240,240,255))");
    mUi->statusbar->showMessage(aWarning);
}

void
MainWindow::on_showInfo(const QString& aInfo)
{

    mStatusBarFadeOutAnimation->start();
    mFadeCycles = 1;

    mUi->statusbar->setStyleSheet("background: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 rgba(0,255,0,0.3), stop:1 rgba(240,240,240,255))");
    mUi->statusbar->showMessage(aInfo);

}

void
MainWindow::on_clearStatusBar()
{
    mUi->statusbar->setStyleSheet("background-color: rgba(240,240,240,255)");
    mUi->statusbar->showMessage("");
}

void
MainWindow::on_statusBarFadeOutFinished()
{
    mStatusBarFadeInAnimation->start();
}

void
MainWindow::on_statusBarFadeInFinished()
{
    if (mFadeCycles >= 1) {
        mStatusBarFadeOutAnimation->start();
        --mFadeCycles;
    }

}
