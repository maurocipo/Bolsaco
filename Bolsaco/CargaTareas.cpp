#include "CargaTareas.h"

#include "DataBaseData.h"
#include "DataBaseUtils.h"
#include "Cortadora.h"
#include "Extrusora.h"
#include "Filtradora.h"
#include "Lavadora.h"
#include "Rebobinadora.h"

#include "ui_CargaTareas.h"

CargaTareas::CargaTareas(QWidget *parent) :
    QWidget(parent),
    mCurrentUser(0),
    mUi(new Ui::CargaTareas)
{
    mUi->setupUi(this);

    // Create Maquinas
    mCortadora = new Cortadora(mUi->widget);
    mExtrusora = new Extrusora(mUi->widget);
    mFiltradora = new Filtradora(mUi->widget);
    mLavadora = new Lavadora(mUi->widget);
    mRebobinadora = new Rebobinadora(mUi->widget);

    // Fill comboboxes
    QStringList tipoDeMaquina = {"Cortadora", "Extrusora", "Filtradora", "Lavadora", "Rebobinadora"};
    mUi->comboBox_tipoDeMaquina->addItems(tipoDeMaquina);
    mUi->comboBox_tipoDeMaquina->setCurrentIndex(-1);

    mUi->comboBox_numero->setCurrentIndex(-1);
    hideMaquinas();
}

CargaTareas::~CargaTareas()
{
    delete mUi;
}

void CargaTareas::on_comboBox_numero_currentIndexChanged(int aMachineNumber)
{
/*    if (aMachineNumber != -1) {
        const int machineIndex = mUi->comboBox_tipoDeMaquina->currentIndex();
        if (machineIndex == DataBaseUtils::Maquinas::CORTADORA) {
            mCortadora->show();
        } else if (machineIndex == DataBaseUtils::Maquinas::EXTRUSORA) {
            mExtrusora->show();
        } else if (machineIndex == DataBaseUtils::Maquinas::FILTRADORA) {
            mFiltradora->show();
        } else if (machineIndex == DataBaseUtils::Maquinas::LAVADORA) {
            mLavadora->show();
        } else if (machineIndex == DataBaseUtils::Maquinas::REBOBINADORA) {
            mRebobinadora->show();
        }
    } else {
        hideMaquinas();
    }
*/
}

void CargaTareas::on_comboBox_tipoDeMaquina_currentIndexChanged(int aMachineIndex)
{
    mUi->comboBox_numero->clear();

    QStringList numeros = {};
    int currentIndex = -1;
    const int cantidadMaquinas = sizeof(DataBaseData::CantidadMaquinas)/sizeof(DataBaseData::CantidadMaquinas[0]);
    if (aMachineIndex > 0 && aMachineIndex < cantidadMaquinas) {
        for (int i = 0; i < DataBaseData::CantidadMaquinas[aMachineIndex]; ++i) {
            numeros.append(QString(i + 1));
        }
    }

    if (aMachineIndex == DataBaseData::TiposMaquinas::LAVADORA) {
        currentIndex = 0;
    } else if (aMachineIndex == DataBaseData::TiposMaquinas::REBOBINADORA) {
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

void CargaTareas::on_pushButton_guardar_pressed()
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
/*    std::vector<DataBaseUtils::KeyAndValue> newData;
    newData.push_back(DataBaseUtils::KeyAndValue(DataBaseUtils::TareaFields::DNI_OPERADOR, QString(mCurrentUser)));

    const int currentMachineIndex = mUi->comboBox_tipoDeMaquina->currentIndex();
    if (currentMachineIndex == DataBaseUtils::Maquinas::CORTADORA) {
        newData.push_back(DataBaseUtils::KeyAndValue(DataBaseUtils::TareaFields::ID_MAQUINA, QString(DataBaseUtils::Maquinas::CORTADORA)));
        std::vector<DataBaseUtils::KeyAndValue> result = mCortadora->getData();
        newData.insert(newData.end(), std::make_move_iterator(result.begin()), std::make_move_iterator(result.end()));
        newData.push_back(DataBaseUtils::KeyAndValue(DataBaseUtils::TareaFields::FECHA, DataBaseUtils::convertDateToYYYYMMDD(QDate::currentDate())));
    } else if (currentMachineIndex == DataBaseUtils::Maquinas::EXTRUSORA) {

    } else if (currentMachineIndex == DataBaseUtils::Maquinas::FILTRADORA) {

    } else if (currentMachineIndex == DataBaseUtils::Maquinas::LAVADORA) {

    } else if (currentMachineIndex == DataBaseUtils::Maquinas::REBOBINADORA) {

    }

    DataBaseUtils::insert(DataBaseUtils::TableNames::TAREA, newData);
*/}
