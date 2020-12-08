#include "CargaTareas.h"

#include "DataBaseData.h"
#include "DataBaseUtils.h"
#include "Cortadora.h"
#include "Extrusora.h"
#include "Filtradora.h"
#include "Lavadora.h"
#include "NotificationSender.h"
#include "Rebobinadora.h"

#include "ui_CargaTareas.h"

using namespace DataBaseUtils;

CargaTareas::CargaTareas(NotificationSender* aNotificationSender, QWidget *parent) :
    QWidget(parent),
    mCurrentUser(0),
    mNotificationSender(aNotificationSender),
    mUi(new Ui::CargaTareas)
{
    mUi->setupUi(this);

    // Create Maquinas Windows
    mCortadora = new Cortadora(mUi->widget);
    mExtrusora = new Extrusora(mUi->widget);
    mFiltradora = new Filtradora(mUi->widget);
    mLavadora = new Lavadora(mUi->widget);
    mRebobinadora = new Rebobinadora(mUi->widget);

    // Fill comboboxes
    mUi->comboBox_tipoDeMaquina->addItems(DataBaseData::TiposMaquinasStr);
    mUi->comboBox_tipoDeMaquina->setCurrentIndex(-1);

    mUi->comboBox_numero->setCurrentIndex(-1);
    hideMaquinas();
}

CargaTareas::~CargaTareas()
{
    delete mUi;
}

void
CargaTareas::on_comboBox_numero_currentIndexChanged(int aMachineNumber)
{
    if (aMachineNumber != -1) {
        const int machineIndex = mUi->comboBox_tipoDeMaquina->currentIndex();
        if (machineIndex == DataBaseData::TiposMaquinas::CORTADORA) {
            mCortadora->fillLargos(buildLargosCortesForDisplay());
            mCortadora->clear();
            mCortadora->show();
        } else if (machineIndex == DataBaseData::TiposMaquinas::EXTRUSORA) {
            mExtrusora->fillMedidas(buildMedidasBobinasForDisplay());
            Result<QString> result = getGratestBobinaId();
            if (result.status() == Status::FAILED) {
                mNotificationSender->emitShowError(result.error());
            }
            mExtrusora->setBobinasIds(result.value());
            mExtrusora->clear();
            mExtrusora->show();
        } else if (machineIndex == DataBaseData::TiposMaquinas::FILTRADORA) {
            mFiltradora->show();
            mFiltradora->clear();
        } else if (machineIndex == DataBaseData::TiposMaquinas::LAVADORA) {
            mLavadora->clear();
            mLavadora->show();
        } else if (machineIndex == DataBaseData::TiposMaquinas::REBOBINADORA) {
            mRebobinadora->clear();
            mRebobinadora->show();
        }
    } else {
        hideMaquinas();
    }

}

void
CargaTareas::on_comboBox_tipoDeMaquina_currentIndexChanged(int aMachineIndex)
{
    mUi->comboBox_numero->clear();

    QStringList numeros = {};
    int currentIndex = -1;
    const int cantTiposMaquinas = sizeof(DataBaseData::CantidadMaquinas)/sizeof(DataBaseData::CantidadMaquinas[0]);
    if ((aMachineIndex >= 0) && (aMachineIndex < cantTiposMaquinas)) {
        for (int i = 0; i < DataBaseData::CantidadMaquinas[aMachineIndex]; ++i) {
            numeros.append(QString::number(i + 1));
        }
    }

    if (DataBaseData::CantidadMaquinas[aMachineIndex] == 1) {
        currentIndex = 0;
    }
    mUi->comboBox_numero->addItems(numeros);
    mUi->comboBox_numero->setCurrentIndex(currentIndex);
}

void
CargaTareas::hideMaquinas() const
{
    mCortadora->hide();
    mExtrusora->hide();
    mFiltradora->hide();
    mLavadora->hide();
    mRebobinadora->hide();
}

void
CargaTareas::on_pushButton_guardar_pressed()
{
    /*
     * Tarea fields:
     * "Id           int     NOT NULL ,"
       "DNI_Operador int     NOT NULL ,"
       "Id_Bobina    int              ,"
       "Id_Bolsa     int              ,"
       "Id_Maquina   int     NOT NULL ,"
       "Fecha        date    NOT NULL ,"
       "Resultado    string  NOT NULL ,"
    */
    Result<QString> stringResult = getCurrentUserId(mCurrentUser);
    if (stringResult.status() != Status::SUCCEEDED) {
        if (stringResult.status() != Status::FAILED) {
            mNotificationSender->emitShowError(stringResult.error());
        } else {
            mNotificationSender->emitShowWarning(stringResult.error());
        }
        return;
    }
    const QString idOperario = stringResult.value();

    const int tipoDeMaquina = mUi->comboBox_tipoDeMaquina->currentIndex();
    if (tipoDeMaquina == -1) {
        mNotificationSender->emitShowWarning("Tipo de Maquina no seleccionada.");
        return;
    }

    stringResult = getCurrentMaquinaId(tipoDeMaquina, mUi->comboBox_numero->currentIndex() + 1);
    if (stringResult.status() != Status::SUCCEEDED) {
        if (stringResult.status() != Status::FAILED) {
            mNotificationSender->emitShowError(stringResult.error());
        } else {
            mNotificationSender->emitShowWarning(stringResult.error());
        }
        return;
    }
    const QString idMaquina = stringResult.value();

    if (tipoDeMaquina == DataBaseData::TiposMaquinas::CORTADORA) {
        Result<void> storeResult = storeTareaCortado(idMaquina, idOperario);
        if (storeResult.status() != Status::SUCCEEDED) {
            if (storeResult.status() == Status::FAILED) {
                mNotificationSender->emitShowError(storeResult.error());
            } else {
                mNotificationSender->emitShowWarning(storeResult.error());
            }
            return;
        }
    } else if (tipoDeMaquina == DataBaseData::TiposMaquinas::EXTRUSORA) {
        Result<void> storeResult = storeTareaExtrusado(idMaquina, idOperario);
        if (storeResult.status() != Status::SUCCEEDED) {
            if (storeResult.status() == Status::FAILED) {
                mNotificationSender->emitShowError(storeResult.error());
            } else {
                mNotificationSender->emitShowWarning(storeResult.error());
            }
            return;
        }
    } else if (tipoDeMaquina == DataBaseData::TiposMaquinas::FILTRADORA) {
        Result<void> storeResult = storeTareaFiltrado(idMaquina, idOperario);
        if (storeResult.status() != Status::SUCCEEDED) {
            if (storeResult.status() == Status::FAILED) {
                mNotificationSender->emitShowError(storeResult.error());
            } else {
                mNotificationSender->emitShowWarning(storeResult.error());
            }
            return;
        }
    } else if (tipoDeMaquina == DataBaseData::TiposMaquinas::LAVADORA) {
        Result<void> storeResult = storeTareaLavado(idMaquina, idOperario);
        if (storeResult.status() != Status::SUCCEEDED) {
            if (storeResult.status() == Status::FAILED) {
                mNotificationSender->emitShowError(storeResult.error());
            } else {
                mNotificationSender->emitShowWarning(storeResult.error());
            }
            return;
        }
    } else if (tipoDeMaquina == DataBaseData::TiposMaquinas::REBOBINADORA) {
        Result<void> storeResult = storeTareaRebobinado(idMaquina, idOperario);
        if (storeResult.status() != Status::SUCCEEDED) {
            if (storeResult.status() == Status::FAILED) {
                mNotificationSender->emitShowError(storeResult.error());
            } else {
                mNotificationSender->emitShowWarning(storeResult.error());
            }
            return;
        }
    }
    mNotificationSender->emitShowInfo("Tarea/s cargada/s correctamente.");
}

QStringList
CargaTareas::buildMedidasBobinasForDisplay() const
{
    QStringList returnData;

    std::vector<KeyAndValue> conditions;
    Result<std::vector<KeyAndValue>> result = select(TableNames::MEDIDAS_BOBINAS, "*", conditions);
    if (result.status() != Status::SUCCEEDED) {
        mNotificationSender->emitShowError(result.error());
        return QStringList();
    }
    std::vector<KeyAndValue> anchos = result.value();

    for (const KeyAndValue& keyAndValue : anchos) {
        if (keyAndValue.mKey == MedidasBobinasFields::ANCHO) {
            returnData.append(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == MedidasBobinasFields::MICRONAJE) {
            QString& medida = returnData.back();
            medida.append("/" + keyAndValue.mValue);
        }
    }
    return returnData;
}

QStringList
CargaTareas::buildLargosCortesForDisplay() const
{
    QStringList returnData;

    std::vector<KeyAndValue> conditions;
    Result<std::vector<KeyAndValue>> result = select(TableNames::MEDIDAS_BOLSAS, MedidasBolsasFields::LARGO, conditions);
    if (result.status() != Status::SUCCEEDED) {
        mNotificationSender->emitShowError(result.error());
        return QStringList();
    }
    std::vector<KeyAndValue> largos = result.value();

    for (const KeyAndValue& keyAndValue : largos) {
        if (keyAndValue.mKey != MedidasBolsasFields::LARGO) {
            mNotificationSender->emitShowError("Error en tabla Medidas de Bolsa.");
            return QStringList();
        }
        returnData.append(keyAndValue.mValue);
    }
    return returnData;
}

Result<void>
CargaTareas::storeTareaCortado(const QString& aIdMaquina, const QString& aIdOperario)
{
    // Usa bobinas creadas en extrusado
    QString largo = mCortadora->getLargo();
    if (largo.isEmpty()) {
        return Result<void>(Status::WARNING, "Largo de Bolsa no seleccionado.");
    }

    std::vector<KeyAndValue> conditions = {KeyAndValue(MedidasBolsasFields::LARGO, largo)};
    Result<std::vector<KeyAndValue>> selectResult = DataBaseUtils::select(TableNames::MEDIDAS_BOLSAS, MedidasBolsasFields::ID, conditions);
    if (selectResult.status() != Status::SUCCEEDED) {
        return Result<void>(Status::FAILED, selectResult.error());
    }
    if (selectResult.value().size() != 1) {
        return Result<void>(Status::FAILED, "Error buscando ID de Medida de Bolsa. Posible entrada duplicada?");
    }
    const QString idLargo = selectResult.value().begin()->mValue;

    std::vector<BobinaIdAndCortes> cortesData = mCortadora->getData();
    if (cortesData.empty()) {
        return Result<void>(Status::WARNING, "Inserte al menos un resultado.");
    }

    for (BobinaIdAndCortes idAndCortes : cortesData) {
        // Validar existencia de Bobina ID
        QString bobinaId = idAndCortes.mBobinaId;
        if (DataBaseUtils::exists(TableNames::BOBINAS, KeyAndValue(BobinasFields::ID, bobinaId)) == false)
        {
            return Result<void>(Status::FAILED, "Error: ID " + bobinaId + " de Bobina inexistente.");
        }
        // Validar que no se haya cargado en otra tarea
        if (DataBaseUtils::exists(TableNames::TAREAS_CORTADO, KeyAndValue(TareaCortadoFields::ID_BOBINA, bobinaId)))
        {
            return Result<void>(Status::FAILED, "Error: ID " + bobinaId + " de Bobina ya cargado en otra tarea de corte.");
        }

        std::vector<KeyAndValue> newData = {KeyAndValue(TareaCortadoFields::CANTIDAD, idAndCortes.mCortes)};
        newData.push_back(KeyAndValue(TareaCortadoFields::FECHA, QDateTime::currentDateTime().toString(dateFormat)));
        newData.push_back(KeyAndValue(TareaCortadoFields::ID_BOBINA, idAndCortes.mBobinaId));
        newData.push_back(KeyAndValue(TareaCortadoFields::ID_MAQUINA, aIdMaquina));
        newData.push_back(KeyAndValue(TareaCortadoFields::ID_MEDIDA_BOLSA, idLargo));
        newData.push_back(KeyAndValue(TareaCortadoFields::ID_OPERARIO, aIdOperario));
        Result<void> insertResult = insert(TableNames::TAREAS_CORTADO, newData);
        if (insertResult.status() != Status::SUCCEEDED) {
            return Result<void>(Status::FAILED, insertResult.error());
        }
    }
    mCortadora->clear();
    return Status::SUCCEEDED;
}

Result<void>
CargaTareas::storeTareaExtrusado(const QString& aIdMaquina, const QString& aIdOperario)
{
    // Crea bobinas (y tareas de extrusado)
    Medida medida = mExtrusora->getMedida();
    if (medida.mAncho.isEmpty() || medida.mMicronaje.isEmpty()) {
        return Result<void>(Status::WARNING, "Ancho/Micronaje no seleccionado.");
    }
    std::vector<KeyAndValue> conditions = {KeyAndValue(MedidasBobinasFields::ANCHO, medida.mAncho),
                                           KeyAndValue(MedidasBobinasFields::MICRONAJE, medida.mMicronaje)};
    Result<std::vector<KeyAndValue>> selectResult = DataBaseUtils::select(TableNames::MEDIDAS_BOBINAS, MedidasBobinasFields::ID, conditions);
    if (selectResult.status() != Status::SUCCEEDED) {
        return Result<void>(Status::FAILED, selectResult.error());
    }
    if (selectResult.value().size() != 1) {
        return Result<void>(Status::FAILED, "Error buscando ID de Medida de Bobina. Posible entrada duplicada?");
    }
    const QString idMedida = selectResult.value().begin()->mValue;

    std::vector<IdAndKilos> bobinasData = mExtrusora->getBobinasData();
    if (bobinasData.empty()) {
        return Result<void>(Status::WARNING, "Inserte al menos un resultado.");
    }

    for (IdAndKilos idAndKilos : bobinasData) {
        std::vector<KeyAndValue> newData{KeyAndValue(BobinasFields::MEDIDA, idMedida)};
        newData.push_back(KeyAndValue(BobinasFields::PESO, idAndKilos.mKilos));
        Result<void> insertResult = insert(TableNames::BOBINAS, newData);
        if (insertResult.status() != Status::SUCCEEDED) {
            return Result<void>(Status::FAILED, insertResult.error());
        }
        newData.clear();

        newData.push_back(KeyAndValue(TareaExtrusadoFields::FECHA, QDateTime::currentDateTime().toString(dateFormat)));
        newData.push_back(KeyAndValue(TareaExtrusadoFields::ID_BOBINA, idAndKilos.mId));
        newData.push_back(KeyAndValue(TareaExtrusadoFields::ID_MAQUINA, aIdMaquina));
        newData.push_back(KeyAndValue(TareaExtrusadoFields::ID_OPERARIO, aIdOperario));
        insertResult = insert(TableNames::TAREAS_EXTRUSADO, newData);
        if (insertResult.status() != Status::SUCCEEDED) {
            return Result<void>(Status::FAILED, insertResult.error());
        }
    }
    mExtrusora->clear();
    Result<QString> result = getGratestBobinaId();
    if (result.status() == Status::FAILED) {
        mNotificationSender->emitShowError(result.error());
    }
    mExtrusora->setBobinasIds(result.value());

    return Status::SUCCEEDED;
}

Result<void>
CargaTareas::storeTareaFiltrado(const QString& aIdMaquina, const QString& aIdOperario)
{
    if (mFiltradora->getKilos() == 0) {
        return Result<void>(Status::WARNING, "Inserte un resultado.");
    }
    QString kilos = QString::number(mFiltradora->getKilos());
    std::vector<KeyAndValue> newData = {KeyAndValue(TareaFiltradoFields::FECHA, QDateTime::currentDateTime().toString(dateFormat))};
    newData.push_back(KeyAndValue(TareaFiltradoFields::ID_MAQUINA, aIdMaquina));
    newData.push_back(KeyAndValue(TareaFiltradoFields::ID_OPERARIO, aIdOperario));
    newData.push_back(KeyAndValue(TareaFiltradoFields::KILOS, kilos));
    Result<void> insertResult = insert(TableNames::TAREAS_FILTRADO, newData);
    if (insertResult.status() != Status::SUCCEEDED) {
        return Result<void>(Status::FAILED, insertResult.error());
    }
    mFiltradora->clear();

    return Status::SUCCEEDED;
}

Result<void>
CargaTareas::storeTareaLavado(const QString& aIdMaquina, const QString& aIdOperario)
{
    std::vector<double> kilosBolsones = mLavadora->getKilosBolsones();
    if (kilosBolsones.empty()) {
        return Result<void>(Status::WARNING, "Inserte al menos un resultado.");
    }

    for (const double& kilos : kilosBolsones) {
        std::vector<KeyAndValue> newData = {KeyAndValue(TareaLavadoFields::FECHA, QDateTime::currentDateTime().toString(dateFormat))};
        newData.push_back(KeyAndValue(TareaLavadoFields::ID_MAQUINA, aIdMaquina));
        newData.push_back(KeyAndValue(TareaLavadoFields::ID_OPERARIO, aIdOperario));
        newData.push_back(KeyAndValue(TareaLavadoFields::KILOS, QString::number(kilos)));
        Result<void> insertResult = insert(TableNames::TAREAS_LAVADO, newData);
        if (insertResult.status() != Status::SUCCEEDED) {
            return Result<void>(Status::FAILED, insertResult.error());
        }
    }
    mLavadora->clear();

    return Status::SUCCEEDED;
}

Result<void>
CargaTareas::storeTareaRebobinado(const QString& aIdMaquina, const QString& aIdOperario)
{
    std::vector<ProductIdAndKilos> productsAndKilos = mRebobinadora->getKilos();
    if (productsAndKilos.empty()) {
        return Result<void>(Status::WARNING, "Inserte al menos un resultado.");
    }

    for (const ProductIdAndKilos& productAndKilos : productsAndKilos) {
        std::vector<KeyAndValue> newData = {KeyAndValue(TareaRebobinadoFields::FECHA, QDateTime::currentDateTime().toString(dateFormat))};
        newData.push_back(KeyAndValue(TareaRebobinadoFields::ID_MAQUINA, aIdMaquina));
        newData.push_back(KeyAndValue(TareaRebobinadoFields::ID_OPERARIO, aIdOperario));
        newData.push_back(KeyAndValue(TareaRebobinadoFields::ID_PRODUCTO_REBOBINADO, QString::number(productAndKilos.mProductId)));
        newData.push_back(KeyAndValue(TareaRebobinadoFields::KILOS, QString::number(productAndKilos.mKilos)));
        Result<void> insertResult = insert(TableNames::TAREAS_REBOBINADO, newData);
        if (insertResult.status() != Status::SUCCEEDED) {
            return Result<void>(Status::FAILED, insertResult.error());
        }
    }
    mRebobinadora->clear();

    return Status::SUCCEEDED;
}


