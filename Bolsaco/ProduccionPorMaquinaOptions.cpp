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
ProduccionPorMaquinaOptions::clear()
{
    mUi->checkBox_FechaDesde->setChecked(false);
    mUi->checkBox_FechaHasta->setChecked(false);

    mUi->comboBox_Maquina->setCurrentIndex(-1);

    mUi->dateTimeEdit_Desde->clear();
    mUi->dateTimeEdit_Hasta->clear();
}

void
ProduccionPorMaquinaOptions::fillComboboxMaquina()
{
    QStringList maquinas;
    mUi->comboBox_Maquina->clear();

    maquinas.append(TODAS);

    int total = 1;
    for (int i = 0; i < DataBaseData::TiposMaquinasStr.size(); ++i) {
        for (int j = 0; j < DataBaseData::CantidadMaquinas[i]; ++j) {
            maquinas.append(QString::number(total).append(": " + DataBaseData::TiposMaquinasStr[i] + " " + QString::number(j + 1)));
            ++total;
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

    const bool todasLasMaquinas = mUi->comboBox_Maquina->currentText() == TODAS;
    const QStringList& maquinaYNumero = mUi->comboBox_Maquina->currentText().split(" ");
    const QString& tipoMaquina = todasLasMaquinas ? "" : maquinaYNumero[1];
    const QString& numeroMaquina = todasLasMaquinas ? "" : maquinaYNumero[2];
    if (todasLasMaquinas ||
        (tipoMaquina == DataBaseData::TiposMaquinasStr[DataBaseData::TiposMaquinas::CORTADORA])) {

        Result<QString> result = getCortadoraTable(todasLasMaquinas, numeroMaquina, desde, hasta);
        if (result.status() != Status::SUCCEEDED) {
            return result;
        }
        htmlString.append(result.value());
    }
    if (todasLasMaquinas ||
        (tipoMaquina == DataBaseData::TiposMaquinasStr[DataBaseData::TiposMaquinas::EXTRUSORA])) {

        Result<QString> result = getExtrusoraTable(todasLasMaquinas, numeroMaquina, desde, hasta);
        if (result.status() != Status::SUCCEEDED) {
            return result;
        }
        htmlString.append(result.value());
    }
    if (todasLasMaquinas ||
        (tipoMaquina == DataBaseData::TiposMaquinasStr[DataBaseData::TiposMaquinas::FILTRADORA])) {

        Result<QString> result = getFiltradoraTable(todasLasMaquinas, numeroMaquina, desde, hasta);
        if (result.status() != Status::SUCCEEDED) {
            return result;
        }
        htmlString.append(result.value());
    }
    if (todasLasMaquinas ||
        (tipoMaquina == DataBaseData::TiposMaquinasStr[DataBaseData::TiposMaquinas::LAVADORA])) {

        Result<QString> result = getLavadoraTable(todasLasMaquinas, numeroMaquina, desde, hasta);
        if (result.status() != Status::SUCCEEDED) {
            return result;
        }
        htmlString.append(result.value());
    }
    if (todasLasMaquinas ||
        (tipoMaquina == DataBaseData::TiposMaquinasStr[DataBaseData::TiposMaquinas::REBOBINADORA])) {

        Result<QString> result = getRebobinadoraTable(todasLasMaquinas, numeroMaquina, desde, hasta);
        if (result.status() != Status::SUCCEEDED) {
            return result;
        }
        htmlString.append(result.value());
    }
    return htmlString;
}

Result<QString>
ProduccionPorMaquinaOptions::getCortadoraTable(const bool aTodasLasMaquinas,
                                               const QString& aNumeroMaquina,
                                               const QString& aDesde,
                                               const QString& aHasta) const
{
    QString htmlString;

    htmlString.append("<h3 style='color: #2e6c80; text-align: center;'>Cortadoras</h3>");
    htmlString.append("<table width = '100%'>");
    htmlString.append("<u><thead><tr>");
    htmlString.append("<th align = center valign = middle>Máquina</th>");
    htmlString.append("<th align = center valign = middle>Total Cortes</th>");
    htmlString.append("<th align = center valign = middle>Total Largo (mts)</th>");
    htmlString.append("</tr></thead></u>");

    std::map<QString, double> cortesPorMaquina;
    std::map<QString, double> largoPorMaquina;

    std::vector<KeyAndValue> condition;
    if (aTodasLasMaquinas == false) {
        Result<QString> maquinaResult = getCurrentMaquinaId(DataBaseData::TiposMaquinas::CORTADORA, aNumeroMaquina.toInt());
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
                                                                    FechaKeyAndValues(TareaCortadoFields::FECHA, aDesde, aHasta));
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

    for (const KeyAndValue& id : condition) {
        htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(id.mValue));
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(cortesPorMaquina[id.mValue]));
        htmlString.append(QString("<td align = center valign = middle>%1</td></tr>\n").arg(largoPorMaquina[id.mValue]));
    }

    htmlString.append("</table>");
    return htmlString;
}

Result<QString>
ProduccionPorMaquinaOptions::getExtrusoraTable(const bool aTodasLasMaquinas,
                                               const QString& aNumeroMaquina,
                                               const QString& aDesde,
                                               const QString& aHasta) const
{
    QString htmlString;

    htmlString.append("<h3 style='color: #2e6c80; text-align: center;'>Extrusoras</h3>");
    htmlString.append("<table width = '100%'>");
    htmlString.append("<u><thead><tr>");
    htmlString.append("<th align = center valign = middle>Máquina</th>");
    htmlString.append("<th align = center valign = middle>Kilos</th>");
    htmlString.append("</tr></thead></u>");

    std::map<QString, double> kilosPorMaquina;

    std::vector<KeyAndValue> condition;
    if (aTodasLasMaquinas == false) {
        Result<QString> maquinaResult = getCurrentMaquinaId(DataBaseData::TiposMaquinas::EXTRUSORA, aNumeroMaquina.toInt());
        if (maquinaResult.status() == Status::FAILED) {
            return maquinaResult;
        }
        condition.push_back(KeyAndValue(TareaExtrusadoFields::ID_MAQUINA, maquinaResult.value()));
    } else {
        for (int i = 1; i <= DataBaseData::CantidadMaquinas[DataBaseData::TiposMaquinas::EXTRUSORA]; ++i) {
            Result<QString> maquinaResult = getCurrentMaquinaId(DataBaseData::TiposMaquinas::EXTRUSORA, i);
            if (maquinaResult.status() == Status::FAILED) {
                return maquinaResult;
            }
            condition.push_back(KeyAndValue(TareaExtrusadoFields::ID_MAQUINA, maquinaResult.value()));
            kilosPorMaquina[maquinaResult.value()] = 0;
        }
    }

    Result<std::vector<KeyAndValue>> selectRes = selectBetweenDates(TableNames::TAREAS_EXTRUSADO,
                                                                    "*",
                                                                    condition,
                                                                    FechaKeyAndValues(TareaExtrusadoFields::FECHA, aDesde, aHasta));
    if(selectRes.status() == Status::FAILED) {
        return Result<QString>(Status::FAILED, selectRes.error());
    }
    std::vector<KeyAndValue> data = selectRes.value();

    MappedTable map;
    for (KeyAndValue keyAndValue : data) {
        if (keyAndValue.mKey == TareaExtrusadoFields::ID_MAQUINA) {
            map[TareaExtrusadoFields::ID_MAQUINA].push_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaExtrusadoFields::ID_BOBINA) {
            map[TareaExtrusadoFields::ID_BOBINA].push_back(keyAndValue.mValue);
        }
    }

    for (size_t i = 0; i < map[TareaExtrusadoFields::ID_MAQUINA].size(); ++i) {
        kilosPorMaquina[map[TareaExtrusadoFields::ID_MAQUINA][i]] += getKilosBobina(map[TareaExtrusadoFields::ID_BOBINA][i]).toDouble();
    }

    for (const KeyAndValue& id : condition) {
        htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(id.mValue));
        htmlString.append(QString("<td align = center valign = middle>%1</td></tr>\n").arg(kilosPorMaquina[id.mValue]));
    }

    htmlString.append("</table>");
    return htmlString;
}

Result<QString>
ProduccionPorMaquinaOptions::getFiltradoraTable(const bool aTodasLasMaquinas,
                                                const QString& aNumeroMaquina,
                                                const QString& aDesde,
                                                const QString& aHasta) const
{
    QString htmlString;

    htmlString.append("<h3 style='color: #2e6c80; text-align: center;'>Filtradoras</h3>");
    htmlString.append("<table width = '100%'>");
    htmlString.append("<u><thead><tr>");
    htmlString.append("<th align = center valign = middle>Máquina</th>");
    htmlString.append("<th align = center valign = middle>Kilos</th>");
    htmlString.append("</tr></thead></u>");

    std::map<QString, double> kilosPorMaquina;

    std::vector<KeyAndValue> condition;
    if (aTodasLasMaquinas == false) {
        Result<QString> maquinaResult = getCurrentMaquinaId(DataBaseData::TiposMaquinas::FILTRADORA, aNumeroMaquina.toInt());
        if (maquinaResult.status() == Status::FAILED) {
            return maquinaResult;
        }
        condition.push_back(KeyAndValue(TareaFiltradoFields::ID_MAQUINA, maquinaResult.value()));
    } else {
        for (int i = 1; i <= DataBaseData::CantidadMaquinas[DataBaseData::TiposMaquinas::FILTRADORA]; ++i) {
            Result<QString> maquinaResult = getCurrentMaquinaId(DataBaseData::TiposMaquinas::FILTRADORA, i);
            if (maquinaResult.status() == Status::FAILED) {
                return maquinaResult;
            }
            condition.push_back(KeyAndValue(TareaFiltradoFields::ID_MAQUINA, maquinaResult.value()));
            kilosPorMaquina[maquinaResult.value()] = 0;
        }
    }

    Result<std::vector<KeyAndValue>> selectRes = selectBetweenDates(TableNames::TAREAS_FILTRADO,
                                                                    "*",
                                                                    condition,
                                                                    FechaKeyAndValues(TareaFiltradoFields::FECHA, aDesde, aHasta));
    if(selectRes.status() == Status::FAILED) {
        return Result<QString>(Status::FAILED, selectRes.error());
    }
    std::vector<KeyAndValue> data = selectRes.value();

    MappedTable map;
    for (KeyAndValue keyAndValue : data) {
        if (keyAndValue.mKey == TareaFiltradoFields::ID_MAQUINA) {
            map[TareaFiltradoFields::ID_MAQUINA].push_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaFiltradoFields::KILOS) {
            map[TareaFiltradoFields::KILOS].push_back(keyAndValue.mValue);
        }
    }

    for (size_t i = 0; i < map[TareaFiltradoFields::ID_MAQUINA].size(); ++i) {
        kilosPorMaquina[map[TareaFiltradoFields::ID_MAQUINA][i]] += map[TareaFiltradoFields::KILOS][i].toDouble();
    }

    for (const KeyAndValue& id : condition) {
        htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(id.mValue));
        htmlString.append(QString("<td align = center valign = middle>%1</td></tr>\n").arg(kilosPorMaquina[id.mValue]));
    }

    htmlString.append("</table>");
    return htmlString;
}

Result<QString>
ProduccionPorMaquinaOptions::getLavadoraTable(const bool aTodasLasMaquinas,
                                              const QString& aNumeroMaquina,
                                              const QString& aDesde,
                                              const QString& aHasta) const
{
    QString htmlString;

    htmlString.append("<h3 style='color: #2e6c80; text-align: center;'>Lavadoras</h3>");
    htmlString.append("<table width = '100%'>");
    htmlString.append("<u><thead><tr>");
    htmlString.append("<th align = center valign = middle>Máquina</th>");
    htmlString.append("<th align = center valign = middle>Kilos</th>");
    htmlString.append("</tr></thead></u>");

    std::map<QString, double> kilosPorMaquina;

    std::vector<KeyAndValue> condition;
    if (aTodasLasMaquinas == false) {
        Result<QString> maquinaResult = getCurrentMaquinaId(DataBaseData::TiposMaquinas::LAVADORA, aNumeroMaquina.toInt());
        if (maquinaResult.status() == Status::FAILED) {
            return maquinaResult;
        }
        condition.push_back(KeyAndValue(TareaLavadoFields::ID_MAQUINA, maquinaResult.value()));
    } else {
        for (int i = 1; i <= DataBaseData::CantidadMaquinas[DataBaseData::TiposMaquinas::LAVADORA]; ++i) {
            Result<QString> maquinaResult = getCurrentMaquinaId(DataBaseData::TiposMaquinas::LAVADORA, i);
            if (maquinaResult.status() == Status::FAILED) {
                return maquinaResult;
            }
            condition.push_back(KeyAndValue(TareaLavadoFields::ID_MAQUINA, maquinaResult.value()));
            kilosPorMaquina[maquinaResult.value()] = 0;
        }
    }

    Result<std::vector<KeyAndValue>> selectRes = selectBetweenDates(TableNames::TAREAS_LAVADO,
                                                                    "*",
                                                                    condition,
                                                                    FechaKeyAndValues(TareaLavadoFields::FECHA, aDesde, aHasta));
    if(selectRes.status() == Status::FAILED) {
        return Result<QString>(Status::FAILED, selectRes.error());
    }
    std::vector<KeyAndValue> data = selectRes.value();

    MappedTable map;
    for (KeyAndValue keyAndValue : data) {
        if (keyAndValue.mKey == TareaLavadoFields::ID_MAQUINA) {
            map[TareaLavadoFields::ID_MAQUINA].push_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaLavadoFields::KILOS) {
            map[TareaLavadoFields::KILOS].push_back(keyAndValue.mValue);
        }
    }

    for (size_t i = 0; i < map[TareaLavadoFields::ID_MAQUINA].size(); ++i) {
        kilosPorMaquina[map[TareaLavadoFields::ID_MAQUINA][i]] += map[TareaLavadoFields::KILOS][i].toDouble();
    }

    for (const KeyAndValue& id : condition) {
        htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(id.mValue));
        htmlString.append(QString("<td align = center valign = middle>%1</td></tr>\n").arg(kilosPorMaquina[id.mValue]));
    }

    htmlString.append("</table>");
    return htmlString;
}

Result<QString>
ProduccionPorMaquinaOptions::getRebobinadoraTable(const bool aTodasLasMaquinas,
                                                  const QString& aNumeroMaquina,
                                                  const QString& aDesde,
                                                  const QString& aHasta) const
{
    QString htmlString;

    htmlString.append("<h3 style='color: #2e6c80; text-align: center;'>Rebobinadoras</h3>");
    htmlString.append("<table width = '100%'>");
    htmlString.append("<u><thead><tr>");
    htmlString.append("<th align = center valign = middle>Máquina</th>");
    htmlString.append("<th align = center valign = middle>Kilos</th>");
    htmlString.append("</tr></thead></u>");

    std::map<QString, double> kilosPorMaquina;

    std::vector<KeyAndValue> condition;
    if (aTodasLasMaquinas == false) {
        Result<QString> maquinaResult = getCurrentMaquinaId(DataBaseData::TiposMaquinas::REBOBINADORA, aNumeroMaquina.toInt());
        if (maquinaResult.status() == Status::FAILED) {
            return maquinaResult;
        }
        condition.push_back(KeyAndValue(TareaRebobinadoFields::ID_MAQUINA, maquinaResult.value()));
    } else {
        for (int i = 1; i <= DataBaseData::CantidadMaquinas[DataBaseData::TiposMaquinas::REBOBINADORA]; ++i) {
            Result<QString> maquinaResult = getCurrentMaquinaId(DataBaseData::TiposMaquinas::REBOBINADORA, i);
            if (maquinaResult.status() == Status::FAILED) {
                return maquinaResult;
            }
            condition.push_back(KeyAndValue(TareaRebobinadoFields::ID_MAQUINA, maquinaResult.value()));
            kilosPorMaquina[maquinaResult.value()] = 0;
        }
    }

    Result<std::vector<KeyAndValue>> selectRes = selectBetweenDates(TableNames::TAREAS_REBOBINADO,
                                                                    "*",
                                                                    condition,
                                                                    FechaKeyAndValues(TareaRebobinadoFields::FECHA, aDesde, aHasta));
    if(selectRes.status() == Status::FAILED) {
        return Result<QString>(Status::FAILED, selectRes.error());
    }
    std::vector<KeyAndValue> data = selectRes.value();

    MappedTable map;
    for (KeyAndValue keyAndValue : data) {
        if (keyAndValue.mKey == TareaRebobinadoFields::ID_MAQUINA) {
            map[TareaRebobinadoFields::ID_MAQUINA].push_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaRebobinadoFields::KILOS) {
            map[TareaRebobinadoFields::KILOS].push_back(keyAndValue.mValue);
        }
    }

    for (size_t i = 0; i < map[TareaRebobinadoFields::ID_MAQUINA].size(); ++i) {
        kilosPorMaquina[map[TareaRebobinadoFields::ID_MAQUINA][i]] += map[TareaRebobinadoFields::KILOS][i].toDouble();
    }

    for (const KeyAndValue& id : condition) {
        htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(id.mValue));
        htmlString.append(QString("<td align = center valign = middle>%1</td></tr>\n").arg(kilosPorMaquina[id.mValue]));
    }

    htmlString.append("</table>");
    return htmlString;
}
