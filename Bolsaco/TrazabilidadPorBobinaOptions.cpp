#include "TrazabilidadPorBobinaOptions.h"
#include "ui_TrazabilidadPorBobinaOptions.h"

#include "DataBaseUtils.h"

using namespace DataBaseUtils;

TrazabilidadPorBobinaOptions::TrazabilidadPorBobinaOptions(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::TrazabilidadPorBobinaOptions)
{
    mUi->setupUi(this);

    mUi->lineEdit_IdsBobinas->setValidator(new QRegExpValidator(QRegExp("[0-9,-]*") , mUi->lineEdit_IdsBobinas));
}

TrazabilidadPorBobinaOptions::~TrazabilidadPorBobinaOptions()
{
    delete mUi;
}

void
TrazabilidadPorBobinaOptions::clear()
{
    mUi->dateTimeEdit_Desde->clear();
    mUi->dateTimeEdit_Hasta->clear();

    mUi->lineEdit_IdsBobinas->clear();
}

Result<QString>
TrazabilidadPorBobinaOptions::getHTMLTable() const
{
    QString htmlString;

    htmlString.append("<h2 style='color: #2e6c80; text-align: center;'>Trazabilidad de Bobinas</h2>");

    QString desde(mUi->dateTimeEdit_Desde->dateTime().toString(dateFormat));
    QString hasta(mUi->dateTimeEdit_Hasta->dateTime().toString(dateFormat));

    htmlString.append("<p style='text-align-last: justify;'><strong>Desde:</strong> " + desde);
    htmlString.append(" &nbsp;&nbsp;&nbsp;&nbsp; <strong>Hasta:</strong> " + hasta);
    htmlString.append(" </p>");

    Result<std::vector<KeyAndValue>> selectRes = selectBetweenDates(TableNames::TAREAS_EXTRUSADO,
                                                                    "*",
                                                                    getBobinasConditions(),
                                                                    FechaKeyAndValues(TareaExtrusadoFields::FECHA, desde, hasta));
    if(selectRes.status() == Status::FAILED) {
        return Result<QString>(Status::FAILED, selectRes.error());
    }
    std::vector<KeyAndValue> data = selectRes.value();

    MappedTable map;
    for (const KeyAndValue& keyAndValue : data) {
        if (keyAndValue.mKey == TareaExtrusadoFields::FECHA) {
            map[TareaExtrusadoFields::FECHA].emplace_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaExtrusadoFields::ID_BOBINA) {
            map[TareaExtrusadoFields::ID_BOBINA].emplace_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaExtrusadoFields::ID_OPERARIO) {
            map[TareaExtrusadoFields::ID_OPERARIO].emplace_back(keyAndValue.mValue);
        }
    }

    for (const QString& idBobina : map[TareaExtrusadoFields::ID_BOBINA]) {
        selectRes = select(TableNames::TAREAS_CORTADO, "*", {KeyAndValue(TareaCortadoFields::ID_BOBINA, idBobina)});
        if(selectRes.status() == Status::FAILED) {
            return Result<QString>(Status::FAILED, selectRes.error());
        }
        std::vector<KeyAndValue> data = selectRes.value();

        if (data.size() > 0) {
            for (const KeyAndValue& keyAndValue : data) {
                if (keyAndValue.mKey == TareaCortadoFields::ID_OPERARIO) {
                    map[QString(TareaCortadoFields::ID_OPERARIO).append("Cortado")].emplace_back(keyAndValue.mValue);
                }
                if (keyAndValue.mKey == TareaCortadoFields::FECHA) {
                    map[QString(TareaCortadoFields::FECHA).append("Cortado")].emplace_back(keyAndValue.mValue);
                }
                if (keyAndValue.mKey == TareaCortadoFields::ID_MEDIDA_BOLSA) {
                    map[QString(TareaCortadoFields::ID_MEDIDA_BOLSA).append("Cortado")].emplace_back(keyAndValue.mValue);
                }
                if (keyAndValue.mKey == TareaCortadoFields::CANTIDAD) {
                    map[QString(TareaCortadoFields::CANTIDAD).append("Cortado")].emplace_back(keyAndValue.mValue);
                }
            }
        } else {
             map[QString(TareaCortadoFields::ID_OPERARIO).append("Cortado")].push_back("");
             map[QString(TareaCortadoFields::FECHA).append("Cortado")].push_back("");
             map[QString(TareaCortadoFields::ID_MEDIDA_BOLSA).append("Cortado")].push_back("");
             map[QString(TareaCortadoFields::CANTIDAD).append("Cortado")].push_back("");
        }
    }

    htmlString.append("<table width = '100%'>");
    htmlString.append("<u><thead><tr>");
    htmlString.append("<th align = center valign = middle>ID_Bobina</th>");
    htmlString.append("<th align = center valign = middle>Responsable Extrusado</th>");
    htmlString.append("<th align = center valign = middle>Responsable Corte</th>");
    htmlString.append("<th align = center valign = middle>Fecha Extrusado</th>");
    htmlString.append("<th align = center valign = middle>Fecha Corte</th>");
    htmlString.append("<th align = center valign = middle>Medida_Bobina (Ancho/Micron)</th>");
    htmlString.append("<th align = center valign = middle>Kilos</th>");
    htmlString.append("<th align = center valign = middle>Largo de Corte</th>");
    htmlString.append("<th align = center valign = middle>Cantidad</th>");///////////////
    htmlString.append("</tr></thead></u>");

    for (size_t i = 0; i < map[TareaExtrusadoFields::ID_BOBINA].size(); ++i) {
        htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(map[TareaExtrusadoFields::ID_BOBINA][i]));
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(getCurrentUserName(getCurrentUserDNI(map[TareaExtrusadoFields::ID_OPERARIO][i]).value()).value()));
        if (map[QString(TareaCortadoFields::ID_OPERARIO).append("Cortado")][i] != "") {
            htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(getCurrentUserName(getCurrentUserDNI(map[QString(TareaCortadoFields::ID_OPERARIO).append("Cortado")][i]).value()).value()));
        } else {
            htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(""));
        }
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(map[TareaExtrusadoFields::FECHA][i]));
        if (map[QString(TareaCortadoFields::FECHA).append("Cortado")][i] != "") {
            htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(map[QString(TareaCortadoFields::FECHA).append("Cortado")][i]));
        } else {
            htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(""));
        }
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(getMedidaBobina(map[TareaExtrusadoFields::ID_BOBINA][i])));
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(getKilosBobina(map[TareaExtrusadoFields::ID_BOBINA][i])));
        if (map[QString(TareaCortadoFields::ID_MEDIDA_BOLSA).append("Cortado")][i] != "") {
            htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(getMedidaBolsa(map[QString(TareaCortadoFields::ID_MEDIDA_BOLSA).append("Cortado")][i])));
        } else {
            htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(""));
        }
        if (map[QString(TareaCortadoFields::CANTIDAD).append("Cortado")][i] != "") {
            htmlString.append(QString("<td align = center valign = middle>%1</td></tr>\n").arg(map[QString(TareaCortadoFields::CANTIDAD).append("Cortado")][i]));
        } else {
            htmlString.append(QString("<td align = center valign = middle>%1</td></tr>\n").arg(""));
        }
    }
    htmlString.append("</table>");
    return htmlString;
}

std::vector<KeyAndValue>
TrazabilidadPorBobinaOptions::getBobinasConditions() const
{
    std::vector<KeyAndValue> returnData;

    if (mUi->lineEdit_IdsBobinas->text().isEmpty() == false) {
        QStringList ids = mUi->lineEdit_IdsBobinas->text().split(",");
        for (QString& id : ids) {
            id = id.simplified();
            id.replace(" ", "");
            if (id.contains("-")) {
                QStringList list2 = id.split(QLatin1Char('-'), Qt::SkipEmptyParts);
                for (int i = list2.begin()->toInt(); i <= list2.back().toInt(); ++i) {
                    returnData.emplace_back(KeyAndValue(TareaExtrusadoFields::ID_BOBINA, QString::number(i)));
                }
            } else {
                returnData.emplace_back(KeyAndValue(TareaExtrusadoFields::ID_BOBINA, id));
            }
        }
    }
    return returnData;
}
