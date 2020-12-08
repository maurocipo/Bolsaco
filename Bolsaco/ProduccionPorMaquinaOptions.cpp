#include "ProduccionPorMaquinaOptions.h"
#include "ui_ProduccionPorMaquinaOptions.h"

#include "DataBaseData.h"

ProduccionPorMaquinaOptions::ProduccionPorMaquinaOptions(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::ProduccionPorMaquinaOptions)
{
    mUi->setupUi(this);

    mUi->comboBox_Maquina->setCurrentIndex(0);

    fillComboboxMaquina();
}

ProduccionPorMaquinaOptions::~ProduccionPorMaquinaOptions()
{
    delete mUi;
}

void
ProduccionPorMaquinaOptions::fillComboboxMaquina()
{
    QStringList maquinas;
    mUi->comboBox_Maquina->clear();

    maquinas.append("TODAS");

    for (int i = 0; i < DataBaseData::TiposMaquinasStr.size(); ++i) {
        for (int j = 0; j < DataBaseData::CantidadMaquinas[i]; ++j) {
            maquinas.append(QString(DataBaseData::TiposMaquinasStr[i] + " " + QString::number(j + 1)));
        }
    }
    mUi->comboBox_Maquina->addItems(maquinas);
}

Result<QString>
ProduccionPorMaquinaOptions::getHTMLTable()
{
    QString htmlString;

/*    htmlString.append("<h2 style='color: #2e6c80; text-align: center;'>Actividad de Empleado</h2>");
    htmlString.append("<p style='text-align-last: justify;'><strong>Nombre: </strong>" + getCurrentUserName(mSelectedOperario).value());
    htmlString.append(" &nbsp;&nbsp;&nbsp;&nbsp; <strong>DNI:</strong> " + mSelectedOperario);

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
        (isTareaEnabled && mUi->comboBox_Tarea->currentIndex() == DataBaseData::TiposMaquinas::CORTADORA)) {

        Result<QString> tareaCortado = buildTareaCortado(userId, desde, hasta);
        if (tareaCortado.status() == Status::FAILED) {
            return tareaCortado;
        }
        htmlString.append(tareaCortado.value());
    }
    if (isTareaEnabled == false ||
        (isTareaEnabled && mUi->comboBox_Tarea->currentIndex() == DataBaseData::TiposMaquinas::EXTRUSORA)) {

        Result<QString> tareaExtrusado = buildTareaExtrusado(userId, desde, hasta);
        if (tareaExtrusado.status() == Status::FAILED) {
            return tareaExtrusado;
        }
        htmlString.append(tareaExtrusado.value());
    }
    if (isTareaEnabled == false ||
        (isTareaEnabled && mUi->comboBox_Tarea->currentIndex() == DataBaseData::TiposMaquinas::FILTRADORA)) {

        Result<QString> tareaFiltrado = buildTareaFiltrado(userId, desde, hasta);
        if (tareaFiltrado.status() == Status::FAILED) {
            return tareaFiltrado;
        }
        htmlString.append(tareaFiltrado.value());
    }
    if (isTareaEnabled == false ||
        (isTareaEnabled && mUi->comboBox_Tarea->currentIndex() == DataBaseData::TiposMaquinas::LAVADORA)) {

        Result<QString> tareaLavado = buildTareaLavado(userId, desde, hasta);
        if (tareaLavado.status() == Status::FAILED) {
            return tareaLavado;
        }
        htmlString.append(tareaLavado.value());
    }
    if (isTareaEnabled == false ||
        (isTareaEnabled && mUi->comboBox_Tarea->currentIndex() == DataBaseData::TiposMaquinas::REBOBINADORA)) {

        Result<QString> tareaRebobinado = buildTareaRebobinado(userId, desde, hasta);
        if (tareaRebobinado.status() == Status::FAILED) {
            return tareaRebobinado;
        }
        htmlString.append(tareaRebobinado.value());
    }*/
    return htmlString;
}
