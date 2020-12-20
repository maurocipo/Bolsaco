#include "ActividadPorEmpleadoOptions.h"
#include "ui_ActividadPorEmpleadoOptions.h"

#include <QDate>
#include <QScrollBar>
#include <QSqlQueryModel>

#include "DataBaseData.h"
#include "DataBaseUtils.h"
#include "NotificationSender.h"

using namespace DataBaseUtils;

ActividadPorEmpleadoOptions::ActividadPorEmpleadoOptions(NotificationSender* aNotificationSender, QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::ActividadPorEmpleadoOptions),
    mNotificationSender(aNotificationSender)
{
    mUi->setupUi(this);

    mUi->lineEdit_DNI->setValidator(new QRegExpValidator(QRegExp("[0-9]*"), mUi->lineEdit_DNI));
    mUi->lineEdit_Nombre->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z]*") , mUi->lineEdit_Nombre));

    mUi->checkBox_FechaDesde->setChecked(false);
    mUi->dateTimeEdit_Desde->setDate(QDate::currentDate());
    mUi->dateTimeEdit_Desde->setEnabled(false);
    mUi->checkBox_FechaHasta->setChecked(false);
    mUi->dateTimeEdit_Hasta->setDate(QDate::currentDate());
    mUi->dateTimeEdit_Hasta->setEnabled(false);

    mUi->checkBox_Maquina->setChecked(false);
    mUi->comboBox_Maquina->setCurrentIndex(0);
    mUi->comboBox_Maquina->setEnabled(false);

    mUi->checkBox_Tarea->setChecked(false);
    mUi->comboBox_Tarea->setCurrentIndex(0);
    mUi->comboBox_Tarea->setEnabled(false);

    fillComboboxTarea();
    fillComboboxMaquina();

    mUi->tableView->horizontalHeader()->setResizeContentsPrecision(QHeaderView::Stretch);
    QSqlQueryModel* model = new QSqlQueryModel();
    QString query("SELECT ");
    query.append(OperariosFields::DNI).append(",").append(OperariosFields::NOMBRE_COMPLETO).append(" FROM ");
    query.append(TableNames::OPERARIOS);
    model->setQuery(query);

    mUi->tableView->setModel(model);
    mUi->tableView->resizeColumnsToContents();
}

ActividadPorEmpleadoOptions::~ActividadPorEmpleadoOptions()
{
    delete mUi;
}

void
ActividadPorEmpleadoOptions::clear()
{
    mUi->checkBox_FechaDesde->setChecked(false);
    mUi->checkBox_FechaHasta->setChecked(false);
    mUi->checkBox_Maquina->setChecked(false);
    mUi->checkBox_Tarea->setChecked(false);

    mUi->comboBox_Maquina->setCurrentIndex(-1);
    mUi->comboBox_Tarea->setCurrentIndex(-1);

    mUi->lineEdit_DNI->clear();
    mUi->lineEdit_Nombre->clear();

    mUi->tableView->update();
}

void
ActividadPorEmpleadoOptions::fillComboboxMaquina()
{
    QStringList maquinas;
    mUi->comboBox_Maquina->clear();

    const int currentTareaIndex = mUi->comboBox_Tarea->currentIndex();
    if (mUi->checkBox_Tarea->isChecked() == false) {
        for (int i = 0; i < DataBaseData::TiposMaquinasStr.size(); ++i) {
            for (int j = 0; j < DataBaseData::CantidadMaquinas[i]; ++j) {
                maquinas.append(QString(DataBaseData::TiposMaquinasStr[i] + " " + QString::number(j + 1)));
            }
        }
    } else {
        for (int j = 0; j < DataBaseData::CantidadMaquinas[currentTareaIndex]; ++j) {
            maquinas.append(QString(DataBaseData::TiposMaquinasStr[currentTareaIndex] + " " + QString::number(j + 1)));
        }
    }
    mUi->comboBox_Maquina->addItems(maquinas);
}

void
ActividadPorEmpleadoOptions::fillComboboxTarea()
{
    QStringList tareas;
    for (QString tarea : DataBaseData::TiposMaquinasStr) {
        if (tarea != "Filmera") {
            tarea.chop(2);
        } else {
            tarea = "Filmado";
        }
        tareas.append(tarea);
    }
    mUi->comboBox_Tarea->addItems(tareas);
}

Result<QString>
ActividadPorEmpleadoOptions::getHTMLTable()
{
    QString htmlString;

    htmlString.append("<h2 style='color: #2e6c80; text-align: center;'>Actividad de Empleado</h2>");
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
    }
    return htmlString;
}

Result<QString>
ActividadPorEmpleadoOptions::buildTareaCortado(const QString& aUserId, const QString& aDesde, const QString& aHasta) const
{
    QString htmlString;
    htmlString.append("<h3 style='color: #2e6c80; text-align: center;'>Cortado</h3>");
    htmlString.append("<table width = '100%'>");
    htmlString.append("<u><thead><tr>");
    htmlString.append("<th align = center valign = middle>Fecha</th>");
    htmlString.append("<th align = center valign = middle>Máquina</th>");
    htmlString.append("<th align = center valign = middle>ID_Bobina</th>");
    htmlString.append("<th align = center valign = middle>Medida_Bobina (Ancho/Micron)</th>");
    htmlString.append("<th align = center valign = middle>Largo_Bolsa</th>");
    htmlString.append("<th align = center valign = middle>Cantidad</th>");
    htmlString.append("</tr></thead></u>");

    // Data
    std::vector<KeyAndValue> condition = {KeyAndValue(TareaCortadoFields::ID_OPERARIO, aUserId)};
    if (mUi->checkBox_Maquina->isChecked()) {
        QStringList list = mUi->comboBox_Maquina->currentText().split(" ");

        if (list.first() != DataBaseData::TiposMaquinasStr[DataBaseData::TiposMaquinas::CORTADORA]) {
            return QString();
        }
        Result<QString> maquinaResult = getCurrentMaquinaId(DataBaseData::TiposMaquinas::CORTADORA, list.back().toInt());
        if (maquinaResult.status() == Status::FAILED) {
            return maquinaResult;
        }
        condition.push_back(KeyAndValue(TareaCortadoFields::ID_MAQUINA, maquinaResult.value()));
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
        if (keyAndValue.mKey == TareaCortadoFields::FECHA) {
            map[TareaCortadoFields::FECHA].push_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaCortadoFields::ID_MAQUINA) {
            map[TareaCortadoFields::ID_MAQUINA].push_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaCortadoFields::ID_BOBINA) {
            map[TareaCortadoFields::ID_BOBINA].push_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaCortadoFields::ID_MEDIDA_BOLSA) {
            map[TareaCortadoFields::ID_MEDIDA_BOLSA].push_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaCortadoFields::CANTIDAD) {
            map[TareaCortadoFields::CANTIDAD].push_back(keyAndValue.mValue);
        }
    }

    double totalCortes = 0;
    for (size_t i = 0; i < map[TareaCortadoFields::FECHA].size(); ++i) {
        htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(map[TareaCortadoFields::FECHA][i]));
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(map[TareaCortadoFields::ID_MAQUINA][i]));
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(map[TareaCortadoFields::ID_BOBINA][i]));
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(getMedidaBobina(map[TareaCortadoFields::ID_BOBINA][i])));
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(getMedidaBolsa(map[TareaCortadoFields::ID_MEDIDA_BOLSA][i])));
        totalCortes += map[TareaCortadoFields::CANTIDAD][i].toDouble();
        htmlString.append(QString("<td align = center valign = middle>%1</td></tr>\n").arg(map[TareaCortadoFields::CANTIDAD][i]));
    }

    htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(""));
    htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(""));
    htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(""));
    htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(""));
    htmlString.append(QString("<td align = center valign = middle>%1</td>").arg("TOTAL CORTES:"));
    htmlString.append(QString("<td align = center valign = middle>%1</td></tr>").arg(QString::number(totalCortes)));

    htmlString.append("</table>");

    return htmlString;
}

Result<QString>
ActividadPorEmpleadoOptions::buildTareaExtrusado(const QString& aUserId, const QString& aDesde, const QString& aHasta) const
{
    QString htmlString;
    htmlString.append("<h3 style='color: #2e6c80; text-align: center;'>Extrusado/Filmado</h3>");
    htmlString.append("<table width = '100%'>");
    htmlString.append("<u><thead><tr>");
    htmlString.append("<th align = center valign = middle>Fecha</th>");
    htmlString.append("<th align = center valign = middle>Máquina</th>");
    htmlString.append("<th align = center valign = middle>ID_Bobina</th>");
    htmlString.append("<th align = center valign = middle>Medida_Bobina (Ancho/Micron)</th>");
    htmlString.append("<th align = center valign = middle>Kilos</th>");
    htmlString.append("</tr></thead></u>");

    // Data
    std::vector<KeyAndValue> condition = {KeyAndValue(TareaExtrusadoFields::ID_OPERARIO, aUserId)};
    if (mUi->checkBox_Maquina->isChecked()) {
        QStringList list = mUi->comboBox_Maquina->currentText().split(" ");

        if (list.first() != DataBaseData::TiposMaquinasStr[DataBaseData::TiposMaquinas::EXTRUSORA]) {
            return QString();
        }
        Result<QString> maquinaResult = getCurrentMaquinaId(DataBaseData::TiposMaquinas::EXTRUSORA, list.back().toInt());
        if (maquinaResult.status() == Status::FAILED) {
            return maquinaResult;
        }
        condition.push_back(KeyAndValue(TareaExtrusadoFields::ID_MAQUINA, maquinaResult.value()));
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
        if (keyAndValue.mKey == TareaExtrusadoFields::FECHA) {
            map[TareaExtrusadoFields::FECHA].push_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaExtrusadoFields::ID_MAQUINA) {
            map[TareaExtrusadoFields::ID_MAQUINA].push_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaExtrusadoFields::ID_BOBINA) {
            map[TareaExtrusadoFields::ID_BOBINA].push_back(keyAndValue.mValue);
        }
    }

    double totalKilos = 0;
    for (size_t i = 0; i < map[TareaCortadoFields::FECHA].size(); ++i) {
        htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(map[TareaExtrusadoFields::FECHA][i]));
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(map[TareaExtrusadoFields::ID_MAQUINA][i]));
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(map[TareaExtrusadoFields::ID_BOBINA][i]));
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(getMedidaBobina(map[TareaExtrusadoFields::ID_BOBINA][i])));
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(getKilosBobina(map[TareaExtrusadoFields::ID_BOBINA][i])));
        totalKilos += getKilosBobina(map[TareaExtrusadoFields::ID_BOBINA][i]).toDouble();
    }

    htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(""));
    htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(""));
    htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(""));
    htmlString.append(QString("<td align = center valign = middle>%1</td>").arg("TOTAL KILOS:"));
    htmlString.append(QString("<td align = center valign = middle>%1</td></tr>").arg(QString::number(totalKilos)));

    htmlString.append("</table>");

    return htmlString;
}

Result<QString>
ActividadPorEmpleadoOptions::buildTareaFiltrado(const QString& aUserId, const QString& aDesde, const QString& aHasta) const
{
    QString htmlString;
    htmlString.append("<h3 style='color: #2e6c80; text-align: center;'>Filtrado</h3>");
    htmlString.append("<table width = '100%'>");
    htmlString.append("<u><thead><tr>");
    htmlString.append("<th align = center valign = middle>Fecha</th>");
    htmlString.append("<th align = center valign = middle>Máquina</th>");
    htmlString.append("<th align = center valign = middle>Kilos</th>");
    htmlString.append("</tr></thead></u>");

    // Data
    std::vector<KeyAndValue> condition = {KeyAndValue(TareaFiltradoFields::ID_OPERARIO, aUserId)};
    if (mUi->checkBox_Maquina->isChecked()) {
        QStringList list = mUi->comboBox_Maquina->currentText().split(" ");

        if (list.first() != DataBaseData::TiposMaquinasStr[DataBaseData::TiposMaquinas::FILTRADORA]) {
            return QString();
        }
        Result<QString> maquinaResult = getCurrentMaquinaId(DataBaseData::TiposMaquinas::FILTRADORA, list.back().toInt());
        if (maquinaResult.status() == Status::FAILED) {
            return maquinaResult;
        }
        condition.push_back(KeyAndValue(TareaFiltradoFields::ID_MAQUINA, maquinaResult.value()));
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
        if (keyAndValue.mKey == TareaFiltradoFields::FECHA) {
            map[TareaFiltradoFields::FECHA].push_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaFiltradoFields::ID_MAQUINA) {
            map[TareaFiltradoFields::ID_MAQUINA].push_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaFiltradoFields::KILOS) {
            map[TareaFiltradoFields::KILOS].push_back(keyAndValue.mValue);
        }
    }

    double totalKilos = 0;
    for (size_t i = 0; i < map[TareaCortadoFields::FECHA].size(); ++i) {
        htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(map[TareaFiltradoFields::FECHA][i]));
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(map[TareaFiltradoFields::ID_MAQUINA][i]));
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(map[TareaFiltradoFields::KILOS][i]));
        totalKilos += map[TareaFiltradoFields::KILOS][i].toDouble();
    }

    htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(""));
    htmlString.append(QString("<td align = center valign = middle>%1</td>").arg("TOTAL KILOS:"));
    htmlString.append(QString("<td align = center valign = middle>%1</td></tr>").arg(QString::number(totalKilos)));

    htmlString.append("</table>");

    return htmlString;
}

Result<QString>
ActividadPorEmpleadoOptions::buildTareaLavado(const QString& aUserId, const QString& aDesde, const QString& aHasta) const
{
    QString htmlString;
    htmlString.append("<h3 style='color: #2e6c80; text-align: center;'>Lavado</h3>");
    htmlString.append("<table width = '100%'>");
    htmlString.append("<u><thead><tr>");
    htmlString.append("<th align = center valign = middle>Fecha</th>");
    htmlString.append("<th align = center valign = middle>Máquina</th>");
    htmlString.append("<th align = center valign = middle>Kilos</th>");
    htmlString.append("</tr></thead></u>");

    // Data
    std::vector<KeyAndValue> condition = {KeyAndValue(TareaLavadoFields::ID_OPERARIO, aUserId)};
    if (mUi->checkBox_Maquina->isChecked()) {
        QStringList list = mUi->comboBox_Maquina->currentText().split(" ");

        if (list.first() != DataBaseData::TiposMaquinasStr[DataBaseData::TiposMaquinas::LAVADORA]) {
            return QString();
        }
        Result<std::vector<KeyAndValue>> result = select(TableNames::MAQUINAS,
                                                         MaquinasFields::ID,
                                                         {KeyAndValue(MaquinasFields::TIPO, QString::number(DataBaseData::TiposMaquinas::LAVADORA)),
                                                          KeyAndValue(MaquinasFields::NUMERO, list.back())});
        if (result.status() == Status::FAILED) {
            return Result<QString>(Status::FAILED, result.error());
        }
        if (result.value().size() > 1) {
            return Result<QString>(Status::FAILED, "Error buscando ID de Máquina.");
        }
        condition.push_back(KeyAndValue(TareaLavadoFields::ID_MAQUINA, result.value().back().mValue));
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
        if (keyAndValue.mKey == TareaLavadoFields::FECHA) {
            map[TareaLavadoFields::FECHA].push_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaLavadoFields::ID_MAQUINA) {
            map[TareaLavadoFields::ID_MAQUINA].push_back(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == TareaLavadoFields::KILOS) {
            map[TareaLavadoFields::KILOS].push_back(keyAndValue.mValue);
        }
    }

    double totalKilos = 0;
    for (size_t i = 0; i < map[TareaCortadoFields::FECHA].size(); ++i) {
        htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(map[TareaLavadoFields::FECHA][i]));
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(map[TareaLavadoFields::ID_MAQUINA][i]));
        htmlString.append(QString("<td align = center valign = middle>%1</td></tr>").arg(map[TareaLavadoFields::KILOS][i]));
        totalKilos += map[TareaLavadoFields::KILOS][i].toDouble();
    }

    htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(""));
    htmlString.append(QString("<td align = center valign = middle>%1</td>").arg("TOTAL KILOS:"));
    htmlString.append(QString("<td align = center valign = middle>%1</td></tr>").arg(QString::number(totalKilos)));
    htmlString.append("</table>");

    return htmlString;
}

Result<QString>
ActividadPorEmpleadoOptions::buildTareaRebobinado(const QString& aUserId, const QString& aDesde, const QString& aHasta) const
{
    QString htmlString;
    htmlString.append("<h3 style='color: #2e6c80; text-align: center;'>Rebobinado</h3>");

    for (int i = 0; i < DataBaseData::ProductosRebobinadoStr.size(); ++i) {
        htmlString.append(QString("<p style='text-align: center;'><strong>%1</strong></p>").arg(DataBaseData::ProductosRebobinadoStr[i]));
        htmlString.append("<table width = '100%'>");
        htmlString.append("<u><thead><tr>");
        htmlString.append("<th align = center valign = middle>Fecha</th>");
        htmlString.append("<th align = center valign = middle>Máquina</th>");
        htmlString.append("<th align = center valign = middle>Kilos</th>");
        htmlString.append("</tr></thead></u>");

        // Data
        std::vector<KeyAndValue> condition = {KeyAndValue(TareaRebobinadoFields::ID_OPERARIO, aUserId),
                                              KeyAndValue(TareaRebobinadoFields::ID_PRODUCTO_REBOBINADO, getProductoRebobinadoId(DataBaseData::ProductosRebobinadoStr[i]))};

        if (mUi->checkBox_Maquina->isChecked()) {
            QStringList list = mUi->comboBox_Maquina->currentText().split(" ");

            if (list.first() != DataBaseData::TiposMaquinasStr[DataBaseData::TiposMaquinas::REBOBINADORA]) {
                return QString();
            }
            Result<std::vector<KeyAndValue>> result = select(TableNames::MAQUINAS,
                                                             MaquinasFields::ID,
                                                             {KeyAndValue(MaquinasFields::TIPO, QString::number(DataBaseData::TiposMaquinas::REBOBINADORA)),
                                                              KeyAndValue(MaquinasFields::NUMERO, list.back())});
            if (result.status() == Status::FAILED) {
                return Result<QString>(Status::FAILED, result.error());
            }
            if (result.value().size() > 1) {
                return Result<QString>(Status::FAILED, "Error buscando ID de Máquina.");
            }
            condition.push_back(KeyAndValue(TareaRebobinadoFields::ID_MAQUINA, result.value().back().mValue));
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
            if (keyAndValue.mKey == TareaRebobinadoFields::FECHA) {
                map[TareaRebobinadoFields::FECHA].push_back(keyAndValue.mValue);
            }
            if (keyAndValue.mKey == TareaRebobinadoFields::ID_MAQUINA) {
                map[TareaRebobinadoFields::ID_MAQUINA].push_back(keyAndValue.mValue);
            }
            if (keyAndValue.mKey == TareaRebobinadoFields::KILOS) {
                map[TareaRebobinadoFields::KILOS].push_back(keyAndValue.mValue);
            }
        }

        double totalKilos = 0;
        for (size_t i = 0; i < map[TareaRebobinadoFields::FECHA].size(); ++i) {
            htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(map[TareaRebobinadoFields::FECHA][i]));
            htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(map[TareaRebobinadoFields::ID_MAQUINA][i]));
            htmlString.append(QString("<td align = center valign = middle>%1</td></tr>").arg(map[TareaRebobinadoFields::KILOS][i]));
            totalKilos += map[TareaRebobinadoFields::KILOS][i].toDouble();
        }

        htmlString.append(QString("<tr><td align = center valign = middle>%1</td>").arg(""));
        htmlString.append(QString("<td align = center valign = middle>%1</td>").arg("TOTAL KILOS:"));
        htmlString.append(QString("<td align = center valign = middle>%1</td></tr>").arg(QString::number(totalKilos)));
        htmlString.append("</table>");
    }
    return htmlString;
}

void
ActividadPorEmpleadoOptions::on_tableView_clicked(const QModelIndex &aIndex)
{
   mSelectedOperario = aIndex.sibling(aIndex.row(), 0).data().toString();
}

void
ActividadPorEmpleadoOptions::on_checkBox_FechaDesde_stateChanged(int arg1)
{
    if (arg1 > 0) {
        mUi->dateTimeEdit_Desde->setEnabled(true);
    } else {
        mUi->dateTimeEdit_Desde->setEnabled(false);
    }
}

void
ActividadPorEmpleadoOptions::on_checkBox_FechaHasta_stateChanged(int arg1)
{
    if (arg1 > 0) {
        mUi->dateTimeEdit_Hasta->setEnabled(true);
    } else {
        mUi->dateTimeEdit_Hasta->setEnabled(false);
    }
}

void
ActividadPorEmpleadoOptions::on_checkBox_Tarea_stateChanged(int arg1)
{
    if (arg1 > 0) {
        mUi->comboBox_Tarea->setEnabled(true);
    } else {
        mUi->comboBox_Tarea->setEnabled(false);
    }
    fillComboboxMaquina();
}

void
ActividadPorEmpleadoOptions::on_checkBox_Maquina_stateChanged(int arg1)
{
    if (arg1 > 0) {
        mUi->comboBox_Maquina->setEnabled(true);
    } else {
        mUi->comboBox_Maquina->setEnabled(false);
    }
}

void
ActividadPorEmpleadoOptions::on_lineEdit_DNI_textChanged(const QString &arg1)
{
    QString query("SELECT ");
    query.append(OperariosFields::DNI).append(",").append(OperariosFields::NOMBRE_COMPLETO).append(" FROM ");
    query.append(TableNames::OPERARIOS).append(" WHERE ");
    query.append(OperariosFields::DNI).append(" LIKE '%").append(arg1 + "%'");
    static_cast<QSqlQueryModel*>(mUi->tableView->model())->setQuery(query);
}

void
ActividadPorEmpleadoOptions::on_lineEdit_Nombre_textChanged(const QString &arg1)
{
    QString query("SELECT ");
    query.append(OperariosFields::DNI).append(",").append(OperariosFields::NOMBRE_COMPLETO).append(" FROM ");
    query.append(TableNames::OPERARIOS).append(" WHERE ");
    query.append(OperariosFields::NOMBRE_COMPLETO).append(" LIKE '%").append(arg1 + "%'");
    static_cast<QSqlQueryModel*>(mUi->tableView->model())->setQuery(query);
}

void
ActividadPorEmpleadoOptions::on_comboBox_Tarea_currentIndexChanged(int /*index*/)
{
    fillComboboxMaquina();
}
