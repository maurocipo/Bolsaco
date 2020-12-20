#include "DatosBobinasOptions.h"
#include "ui_DatosBobinasOptions.h"

#include <set>

#include "DataBaseUtils.h"
#include "NotificationSender.h"

using namespace DataBaseUtils;

DatosBobinasOptions::DatosBobinasOptions(NotificationSender* aNotificationSender, QWidget *parent) :
    QWidget(parent),
    mNotificationSender(aNotificationSender),
    mUi(new Ui::DatosBobinasOptions)
{
    mUi->setupUi(this);
}

DatosBobinasOptions::~DatosBobinasOptions()
{
    delete mUi;
}

void
DatosBobinasOptions::clear()
{
    mUi->spinBox_Ancho->clear();
    mUi->spinBox_Largo->clear();
    mUi->spinBox_Micronaje->clear();
}

void DatosBobinasOptions::on_toolButton_Ancho_pressed()
{
    const QString nuevoAncho = mUi->spinBox_Ancho->text();

    if (exists(TableNames::MEDIDAS_BOBINAS, KeyAndValue(MedidasBobinasFields::ANCHO, nuevoAncho))) {
        mNotificationSender->emitShowError("Ancho de Bobina ya existente.");
        return;
    }

    Result<std::vector<KeyAndValue>> resultSelect = select(TableNames::MEDIDAS_BOBINAS, MedidasBobinasFields::MICRONAJE, {});
    if (resultSelect.status() != Status::SUCCEEDED) {
        mNotificationSender->emitShowError(resultSelect.error());
        return;
    }

    std::set<QString> micronajes;
    for (const KeyAndValue& micronaje : resultSelect.value()) {
        micronajes.insert(micronaje.mValue);
    }

    for (const QString& micronaje : micronajes) {
        Result<void> resultInsert = insert(TableNames::MEDIDAS_BOBINAS, {KeyAndValue(MedidasBobinasFields::ANCHO, nuevoAncho),
                                                                         KeyAndValue(MedidasBobinasFields::MICRONAJE, micronaje)});
        if (resultInsert.status() != Status::SUCCEEDED) {
            mNotificationSender->emitShowError(resultInsert.error());
            return;
        }
    }

    mNotificationSender->emitShowInfo("Ancho de Bobina guardado con éxito!");
    mUi->spinBox_Ancho->clear();
}

void DatosBobinasOptions::on_toolButton_Micronaje_pressed()
{
    const QString nuevoMicronaje = mUi->spinBox_Ancho->text();

    if (exists(TableNames::MEDIDAS_BOBINAS, KeyAndValue(MedidasBobinasFields::MICRONAJE, nuevoMicronaje))) {
        mNotificationSender->emitShowError("Micronaje de Bobina ya existente.");
        return;
    }

    Result<std::vector<KeyAndValue>> resultSelect = select(TableNames::MEDIDAS_BOBINAS, MedidasBobinasFields::ANCHO, {});
    if (resultSelect.status() != Status::SUCCEEDED) {
        mNotificationSender->emitShowError(resultSelect.error());
        return;
    }

    std::set<QString> anchos;
    for (const KeyAndValue& ancho : resultSelect.value()) {
        anchos.insert(ancho.mValue);
    }

    for (const QString& ancho : anchos) {
        Result<void> resultInsert = insert(TableNames::MEDIDAS_BOBINAS, {KeyAndValue(MedidasBobinasFields::ANCHO, ancho),
                                                                         KeyAndValue(MedidasBobinasFields::MICRONAJE, nuevoMicronaje)});
        if (resultInsert.status() != Status::SUCCEEDED) {
            mNotificationSender->emitShowError(resultInsert.error());
            return;
        }
    }

    mNotificationSender->emitShowInfo("Micronaje de Bobina guardado con éxito!");
    mUi->spinBox_Micronaje->clear();
}

void DatosBobinasOptions::on_toolButton_Largo_pressed()
{
    const QString nuevoLargo = mUi->spinBox_Largo->text();

    if (exists(TableNames::MEDIDAS_BOLSAS, KeyAndValue(MedidasBolsasFields::LARGO, nuevoLargo))) {
        mNotificationSender->emitShowError("Largo de Corte ya existente.");
        return;
    }

    Result<void> resultInsert = insert(TableNames::MEDIDAS_BOLSAS, {KeyAndValue(MedidasBolsasFields::LARGO, nuevoLargo)});
    if (resultInsert.status() != Status::SUCCEEDED) {
        mNotificationSender->emitShowError(resultInsert.error());
        return;
    }

    mNotificationSender->emitShowInfo("Nuevo Largo de Corte guardado con éxito!");
    mUi->spinBox_Largo->clear();
}
