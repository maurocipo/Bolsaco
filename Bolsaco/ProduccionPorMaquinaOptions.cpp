#include "ProduccionPorMaquinaOptions.h"
#include "ui_ProduccionPorMaquinaOptions.h"

#include "DataBaseData.h"
#include "DataBaseUtils.h"

using namespace DataBaseUtils;

const char* TODAS = "TODAS";

ProduccionPorMaquinaOptions::ProduccionPorMaquinaOptions(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::ProduccionPorMaquinaOptions)
{
    mUi->setupUi(this);

    mUi->checkBox_FechaDesde->setChecked(false);
    mUi->dateTimeEdit_Desde->setDate(QDate::currentDate());
    mUi->dateTimeEdit_Desde->setEnabled(false);
    mUi->checkBox_FechaHasta->setChecked(false);
    mUi->dateTimeEdit_Hasta->setDate(QDate::currentDate());
    mUi->dateTimeEdit_Hasta->setEnabled(false);

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

    maquinas.append(TODAS);

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

    htmlString.append("<h2 style='color: #2e6c80; text-align: center;'>Producción por Máquina</h2>");

    QString desde(mUi->dateTimeEdit_Desde->dateTime().toString(dateFormat));
    QString hasta(mUi->dateTimeEdit_Hasta->dateTime().toString(dateFormat));
    htmlString.append("<p style='text-align-last: justify;'>");
    if (mUi->checkBox_FechaDesde->isChecked()) {
        htmlString.append("<strong>Desde:</strong> " + desde);
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

    htmlString.append("<h3 style='color: #2e6c80; text-align: center;'>Cortadoras</h3>");
    htmlString.append("<table width = '100%'>");
    htmlString.append("<u><thead><tr>");
    htmlString.append("<th align = center valign = middle>Máquina</th>");
    htmlString.append("<th align = center valign = middle>Total Cortes</th>");
    htmlString.append("<th align = center valign = middle>Total Largo (mts)</th>");
    htmlString.append("</tr></thead></u>");

    const bool todasLasMaquinas = mUi->comboBox_Maquina->currentText() == TODAS;
    QStringList maquinaYNumero = mUi->comboBox_Maquina->currentText().split(" ");
    if (todasLasMaquinas ||
        (maquinaYNumero.first() == DataBaseData::TiposMaquinasStr[DataBaseData::TiposMaquinas::CORTADORA])) {

        std::map<QString, double> cortesPorMaquina;
        std::map<QString, double> largoPorMaquina;

        std::vector<KeyAndValue> condition;
        if (todasLasMaquinas == false) {
            Result<QString> maquinaResult = getCurrentMaquinaId(DataBaseData::TiposMaquinas::CORTADORA, maquinaYNumero.back().toInt());
            if (maquinaResult.status() == Status::FAILED) {
                return maquinaResult;
            }
            condition.push_back(KeyAndValue(TareaCortadoFields::ID_MAQUINA, maquinaResult.value()));
        } else {
            for (int i = 1; i <= DataBaseData::CantidadMaquinas[DataBaseData::TiposMaquinas::CORTADORA]; ++i) {
                Result<QString> maquinaResult = getCurrentMaquinaId(DataBaseData::TiposMaquinas::CORTADORA, i);
                if (maquinaResult.status() == Status::FAILED) {
                    return maquinaResult;
                }
                condition.push_back(KeyAndValue(TareaCortadoFields::ID_MAQUINA, maquinaResult.value()));
                cortesPorMaquina[maquinaResult.value()] = 0;
                largoPorMaquina[maquinaResult.value()] = 0;
            }
        }

        Result<std::vector<KeyAndValue>> selectRes = selectBetweenDates(TableNames::TAREAS_CORTADO,
                                                                        "*",
                                                                        condition,
                                                                        FechaKeyAndValues(TareaCortadoFields::FECHA, desde, hasta));
        if(selectRes.status() == Status::FAILED) {
            return Result<QString>(Status::FAILED, selectRes.error());
        }
        std::vector<KeyAndValue> data = selectRes.value();

        MappedTable map;
        for (KeyAndValue keyAndValue : data) {
            if (keyAndValue.mKey == TareaCortadoFields::ID_MAQUINA) {
                map[TareaCortadoFields::ID_MAQUINA].push_back(keyAndValue.mValue);
            }
            if (keyAndValue.mKey == TareaCortadoFields::ID_MEDIDA_BOLSA) {
                map[TareaCortadoFields::ID_MEDIDA_BOLSA].push_back(keyAndValue.mValue);
            }
            if (keyAndValue.mKey == TareaCortadoFields::CANTIDAD) {
                map[TareaCortadoFields::CANTIDAD].push_back(keyAndValue.mValue);
            }
        }

        for (size_t i = 0; i < map[TareaCortadoFields::ID_MAQUINA].size(); ++i) {
            cortesPorMaquina[map[TareaCortadoFields::ID_MAQUINA][i]] += map[TareaCortadoFields::CANTIDAD][i].toDouble();
            largoPorMaquina[map[TareaCortadoFields::ID_MAQUINA][i]] += getMedidaBolsa(map[TareaCortadoFields::ID_MEDIDA_BOLSA][i]).toDouble() / 100 * map[TareaCortadoFields::CANTIDAD][i].toDouble();
        }


        for (std::pair<QString, double> idYCortes : cortesPorMaquina) {
            htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(idYCortes.first));
            htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(idYCortes.second));
            htmlString.append(QString("<td align = center valign = middle>%1</td></tr>\n").arg(largoPorMaquina[idYCortes.first]));
        }

    }/*
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
