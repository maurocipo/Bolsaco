#include "DataBaseUtils.h"

#include <iostream>

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include "NotificationSender.h"

const char* DataBaseUtils::dateFormat = "dd.MM.yyyy hh:mm:ss";

const char* DataBaseUtils::TableNames::BOBINAS = "Bobinas";
const char* DataBaseUtils::TableNames::MAQUINAS = "Maquinas";
const char* DataBaseUtils::TableNames::MEDIDAS_BOBINAS = "Medidas_Bobinas";
const char* DataBaseUtils::TableNames::MEDIDAS_BOLSAS = "Medidas_Bolsas";
const char* DataBaseUtils::TableNames::OPERARIOS = "Operarios";
const char* DataBaseUtils::TableNames::PRODUCTOS_REBOBINADO = "Productos_Rebobinado";
const char* DataBaseUtils::TableNames::TAREAS_CORTADO = "Tareas_Cortado";
const char* DataBaseUtils::TableNames::TAREAS_EXTRUSADO = "Tareas_Extrusado";
const char* DataBaseUtils::TableNames::TAREAS_FILTRADO = "Tareas_Filtrado";
const char* DataBaseUtils::TableNames::TAREAS_LAVADO = "Tareas_Lavado";
const char* DataBaseUtils::TableNames::TAREAS_REBOBINADO = "Tareas_Rebobinado";
const char* DataBaseUtils::TableNames::TIPOS_MAQUINAS = "Tipos_Maquinas";

const char* DataBaseUtils::BobinasFields::ID = "Id_Bobina";
const char* DataBaseUtils::BobinasFields::MEDIDA = "Medida";
const char* DataBaseUtils::BobinasFields::PESO = "Peso";

const char* DataBaseUtils::MaquinasFields::DESCRIPCION = "Descripcion";
const char* DataBaseUtils::MaquinasFields::ID = "Id_Maquina";
const char* DataBaseUtils::MaquinasFields::NUMERO = "Numero";
const char* DataBaseUtils::MaquinasFields::TIPO = DataBaseUtils::TiposMaquinasFields::ID;

const char* DataBaseUtils::MedidasBobinasFields::ANCHO = "Ancho";
const char* DataBaseUtils::MedidasBobinasFields::ID = "Id_Medida_Bobina";
const char* DataBaseUtils::MedidasBobinasFields::MICRONAJE = "Micronaje";

const char* DataBaseUtils::MedidasBolsasFields::ID = "Id_Medida_Bolsa";
const char* DataBaseUtils::MedidasBolsasFields::LARGO = "Largo";

const char* DataBaseUtils::OperariosFields::DNI = "DNI";
const char* DataBaseUtils::OperariosFields::ID = "Id_Operario";
const char* DataBaseUtils::OperariosFields::NOMBRE_COMPLETO = "Nombre_Completo";

const char* DataBaseUtils::ProductosRebobinadoFields::DESCRIPCION = "Descripcion";
const char* DataBaseUtils::ProductosRebobinadoFields::ID = "Id_Producto_Rebobinado";

const char* DataBaseUtils::TareaCortadoFields::CANTIDAD = "Cantidad";
const char* DataBaseUtils::TareaCortadoFields::FECHA = "Fecha";
const char* DataBaseUtils::TareaCortadoFields::ID = "Id_Tarea_Cortado";
const char* DataBaseUtils::TareaCortadoFields::ID_BOBINA = DataBaseUtils::BobinasFields::ID;
const char* DataBaseUtils::TareaCortadoFields::ID_MAQUINA = DataBaseUtils::MaquinasFields::ID;
const char* DataBaseUtils::TareaCortadoFields::ID_MEDIDA_BOLSA = DataBaseUtils::MedidasBolsasFields::ID;
const char* DataBaseUtils::TareaCortadoFields::ID_OPERARIO = DataBaseUtils::OperariosFields::ID;

const char* DataBaseUtils::TareaExtrusadoFields::FECHA = "Fecha";
const char* DataBaseUtils::TareaExtrusadoFields::ID = "Id_Tarea_Extrusado";
const char* DataBaseUtils::TareaExtrusadoFields::ID_BOBINA = DataBaseUtils::BobinasFields::ID;
const char* DataBaseUtils::TareaExtrusadoFields::ID_MAQUINA = DataBaseUtils::MaquinasFields::ID;
const char* DataBaseUtils::TareaExtrusadoFields::ID_OPERARIO = DataBaseUtils::OperariosFields::ID;

const char* DataBaseUtils::TareaFiltradoFields::FECHA = "Fecha";
const char* DataBaseUtils::TareaFiltradoFields::ID = "Id_Tarea_Filtrado";
const char* DataBaseUtils::TareaFiltradoFields::ID_MAQUINA = DataBaseUtils::MaquinasFields::ID;
const char* DataBaseUtils::TareaFiltradoFields::ID_OPERARIO = DataBaseUtils::OperariosFields::ID;
const char* DataBaseUtils::TareaFiltradoFields::KILOS = "Kilos";

const char* DataBaseUtils::TareaLavadoFields::FECHA = "Fecha";
const char* DataBaseUtils::TareaLavadoFields::ID = "Id_Tarea_Lavado";
const char* DataBaseUtils::TareaLavadoFields::ID_MAQUINA = DataBaseUtils::MaquinasFields::ID;
const char* DataBaseUtils::TareaLavadoFields::ID_OPERARIO = DataBaseUtils::OperariosFields::ID;
const char* DataBaseUtils::TareaLavadoFields::KILOS = "Kilos";

const char* DataBaseUtils::TareaRebobinadoFields::FECHA = "Fecha";
const char* DataBaseUtils::TareaRebobinadoFields::ID = "Id_Tarea_Rebobinado";
const char* DataBaseUtils::TareaRebobinadoFields::ID_MAQUINA = DataBaseUtils::MaquinasFields::ID;
const char* DataBaseUtils::TareaRebobinadoFields::ID_OPERARIO = DataBaseUtils::OperariosFields::ID;
const char* DataBaseUtils::TareaRebobinadoFields::ID_PRODUCTO_REBOBINADO = DataBaseUtils::ProductosRebobinadoFields::ID;
const char* DataBaseUtils::TareaRebobinadoFields::KILOS = "Kilos";

const char* DataBaseUtils::TiposMaquinasFields::ID = "Id_Tipo_de_Maquina";
const char* DataBaseUtils::TiposMaquinasFields::TIPO = "Tipo";

const std::string createBobinasStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::BOBINAS) +
                                     "(" +
                                     std::string(DataBaseUtils::BobinasFields::ID)     + " integer NOT NULL," +
                                     std::string(DataBaseUtils::BobinasFields::MEDIDA) + " string  NOT NULL," +
                                     std::string(DataBaseUtils::BobinasFields::PESO)   + " float   NOT NULL," +
                                     "PRIMARY KEY (" + std::string(DataBaseUtils::BobinasFields::ID) + ")" +
                                     ");";
const char* DataBaseUtils::CreationCommands::createBobinas = createBobinasStr.c_str();

const std::string createMaquinasStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::MAQUINAS) +
                                      "(" +
                                      std::string(DataBaseUtils::MaquinasFields::DESCRIPCION) + " string          ," +
                                      std::string(DataBaseUtils::MaquinasFields::ID)          + " integer NOT NULL," +
                                      std::string(DataBaseUtils::MaquinasFields::NUMERO)      + " integer NOT NULL," +
                                      std::string(DataBaseUtils::MaquinasFields::TIPO)        + " integer NOT NULL," +
                                      "PRIMARY KEY (" + std::string(DataBaseUtils::MaquinasFields::ID) + ")" +
                                      ");";
const char* DataBaseUtils::CreationCommands::createMaquinas = createMaquinasStr.c_str();

const std::string createMedidasBobinasStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::MEDIDAS_BOBINAS) +
                                            "(" +
                                            std::string(DataBaseUtils::MedidasBobinasFields::ANCHO)     + " integer NOT NULL," +
                                            std::string(DataBaseUtils::MedidasBobinasFields::ID)        + " integer NOT NULL," +
                                            std::string(DataBaseUtils::MedidasBobinasFields::MICRONAJE) + " integer NOT NULL," +
                                            "PRIMARY KEY (" + std::string(DataBaseUtils::MedidasBobinasFields::ID) + ")" +
                                            ");";
const char* DataBaseUtils::CreationCommands::createMedidasBobinas = createMedidasBobinasStr.c_str();

const std::string createMedidasBolsasStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::MEDIDAS_BOLSAS) +
                                           "(" +
                                           std::string(DataBaseUtils::MedidasBolsasFields::ID)    + " integer NOT NULL," +
                                           std::string(DataBaseUtils::MedidasBolsasFields::LARGO) + " integer NOT NULL," +
                                           "PRIMARY KEY (" + std::string(DataBaseUtils::MedidasBolsasFields::ID) + ")" +
                                           ");";
const char* DataBaseUtils::CreationCommands::createMedidasBolsas = createMedidasBolsasStr.c_str();

const std::string createOperariosStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::OPERARIOS) +
                                       "(" +
                                       std::string(DataBaseUtils::OperariosFields::DNI)             + " integer NOT NULL," +
                                       std::string(DataBaseUtils::OperariosFields::ID)              + " integer NOT NULL," +
                                       std::string(DataBaseUtils::OperariosFields::NOMBRE_COMPLETO) + " string  NOT NULL," +
                                       "PRIMARY KEY (" + std::string(DataBaseUtils::OperariosFields::ID) + ")" +
                                       ");";
const char* DataBaseUtils::CreationCommands::createOperarios = createOperariosStr.c_str();

const std::string createProductosRebobinadoStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::PRODUCTOS_REBOBINADO) +
                                                 "(" +
                                                 std::string(DataBaseUtils::ProductosRebobinadoFields::DESCRIPCION) + " string  NOT NULL," +
                                                 std::string(DataBaseUtils::ProductosRebobinadoFields::ID)          + " integer NOT NULL," +
                                                 "PRIMARY KEY (" + std::string(DataBaseUtils::ProductosRebobinadoFields::ID) + ")" +
                                                 ");";
const char* DataBaseUtils::CreationCommands::createProductosRebobinado = createProductosRebobinadoStr.c_str();

const std::string createTareasCortadoStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::TAREAS_CORTADO) +
                                           "(" +
                                           std::string(DataBaseUtils::TareaCortadoFields::CANTIDAD)        + " integer NOT NULL," +
                                           std::string(DataBaseUtils::TareaCortadoFields::FECHA)           + " string    NOT NULL," +
                                           std::string(DataBaseUtils::TareaCortadoFields::ID)              + " integer NOT NULL," +
                                           std::string(DataBaseUtils::TareaCortadoFields::ID_BOBINA)       + " integer NOT NULL," +
                                           std::string(DataBaseUtils::TareaCortadoFields::ID_MAQUINA)      + " integer NOT NULL," +
                                           std::string(DataBaseUtils::TareaCortadoFields::ID_MEDIDA_BOLSA) + " integer NOT NULL," +
                                           std::string(DataBaseUtils::TareaCortadoFields::ID_OPERARIO)     + " integer NOT NULL," +
                                           "PRIMARY KEY (" + std::string(DataBaseUtils::TareaCortadoFields::ID) + ")" +
                                           ");";
const char* DataBaseUtils::CreationCommands::createTareasCortado = createTareasCortadoStr.c_str();

const std::string createTareasExtrusadoStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::TAREAS_EXTRUSADO) +
                                             "(" +
                                             std::string(DataBaseUtils::TareaExtrusadoFields::FECHA)       + " string    NOT NULL," +
                                             std::string(DataBaseUtils::TareaExtrusadoFields::ID)          + " integer NOT NULL," +
                                             std::string(DataBaseUtils::TareaExtrusadoFields::ID_BOBINA)   + " integer NOT NULL," +
                                             std::string(DataBaseUtils::TareaExtrusadoFields::ID_MAQUINA)  + " integer NOT NULL," +
                                             std::string(DataBaseUtils::TareaExtrusadoFields::ID_OPERARIO) + " integer NOT NULL," +
                                             "PRIMARY KEY (" + std::string(DataBaseUtils::TareaExtrusadoFields::ID) + ")" +
                                             ");";
const char* DataBaseUtils::CreationCommands::createTareasExtrusado = createTareasExtrusadoStr.c_str();

const std::string createTareasFiltradoStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::TAREAS_FILTRADO) +
                                            "(" +
                                            std::string(DataBaseUtils::TareaFiltradoFields::FECHA)       + " string    NOT NULL," +
                                            std::string(DataBaseUtils::TareaFiltradoFields::ID)          + " integer NOT NULL," +
                                            std::string(DataBaseUtils::TareaFiltradoFields::ID_MAQUINA)  + " integer NOT NULL," +
                                            std::string(DataBaseUtils::TareaFiltradoFields::ID_OPERARIO) + " integer NOT NULL," +
                                            std::string(DataBaseUtils::TareaFiltradoFields::KILOS)       + " integer NOT NULL," +
                                            "PRIMARY KEY (" + std::string(DataBaseUtils::TareaFiltradoFields::ID) + ")" +
                                            ");";
const char* DataBaseUtils::CreationCommands::createTareasFiltrado = createTareasFiltradoStr.c_str();

const std::string createTareasLavadoStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::TAREAS_LAVADO) +
                                          "(" +
                                          std::string(DataBaseUtils::TareaLavadoFields::FECHA)       + " date    NOT NULL," +
                                          std::string(DataBaseUtils::TareaLavadoFields::ID)          + " integer NOT NULL," +
                                          std::string(DataBaseUtils::TareaLavadoFields::ID_MAQUINA)  + " integer NOT NULL," +
                                          std::string(DataBaseUtils::TareaLavadoFields::ID_OPERARIO) + " integer NOT NULL," +
                                          std::string(DataBaseUtils::TareaLavadoFields::KILOS)       + " integer NOT NULL," +
                                          "PRIMARY KEY (" + std::string(DataBaseUtils::TareaLavadoFields::ID) + ")" +
                                          ");";
const char* DataBaseUtils::CreationCommands::createTareasLavado = createTareasLavadoStr.c_str();

const std::string createTareasRebobinadoStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::TAREAS_REBOBINADO) +
                                              "(" +
                                              std::string(DataBaseUtils::TareaRebobinadoFields::FECHA)                    + " string    NOT NULL," +
                                              std::string(DataBaseUtils::TareaRebobinadoFields::ID)                       + " integer NOT NULL," +
                                              std::string(DataBaseUtils::TareaRebobinadoFields::ID_MAQUINA)               + " integer NOT NULL," +
                                              std::string(DataBaseUtils::TareaRebobinadoFields::ID_OPERARIO)              + " integer NOT NULL," +
                                              std::string(DataBaseUtils::TareaRebobinadoFields::ID_PRODUCTO_REBOBINADO)   + " integer NOT NULL," +
                                              std::string(DataBaseUtils::TareaRebobinadoFields::KILOS)                    + " double  NOT NULL," +
                                              "PRIMARY KEY (" + std::string(DataBaseUtils::TareaRebobinadoFields::ID) + ")" +
                                              ");";
const char* DataBaseUtils::CreationCommands::createTareasRebobinado = createTareasRebobinadoStr.c_str();

const std::string createTiposMaquinasStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::TIPOS_MAQUINAS) +
                                           "(" +
                                           std::string(DataBaseUtils::TiposMaquinasFields::ID)   + " integer NOT NULL," +
                                           std::string(DataBaseUtils::TiposMaquinasFields::TIPO) + " string  NOT NULL," +
                                           "PRIMARY KEY (" + std::string(DataBaseUtils::TiposMaquinasFields::ID) + ")" +
                                           ");";
const char* DataBaseUtils::CreationCommands::createTiposMaquinas = createTiposMaquinasStr.c_str();

QString
DataBaseUtils::buildSelectQuery(const QString& aTable, const QString& aColumn, const std::vector<KeyAndValue>& aConditions)
{
    QString queryStr;
    queryStr.append("SELECT " + aColumn + " FROM " + aTable + " WHERE ");

    if (aConditions.empty() == false) {
        QStringList keys;
        for(const KeyAndValue& keyAndValue : aConditions) {
            keys.append(keyAndValue.mKey);
        }
        for(const KeyAndValue& keyAndValue : aConditions) {
            keys.pop_front();
            if (keys.contains(keyAndValue.mKey)) {
                queryStr.append(keyAndValue.mKey + " = '" + keyAndValue.mValue + "' OR  ");
            } else {
                queryStr.append(keyAndValue.mKey + " = '" + keyAndValue.mValue + "' AND ");
            }
        }
        queryStr.chop(5);
    } else {
        queryStr.chop(7);
    }
    return queryStr;
}

Result<std::vector<DataBaseUtils::KeyAndValue>>
DataBaseUtils::select(const QString& aTable, const QString& aColumn, const std::vector<KeyAndValue>& aConditions)
{
    return internalSelect(aTable, buildSelectQuery(aTable, aColumn, aConditions));
}

Result<std::vector<DataBaseUtils::KeyAndValue>>
DataBaseUtils::selectBetweenDates(const QString& aTable, const QString& aColumn, const std::vector<KeyAndValue>& aConditions, const FechaKeyAndValues& aDates)
{
    QString query = buildSelectQuery(aTable, aColumn, aConditions);
    QString desde = aDates.mDesde;
    QString hasta = aDates.mHasta;
    if (aConditions.empty()) {
        query.append(" WHERE (");
    } else {
        query.append(" AND (");
    }
    query.append(aDates.mKey).append(" BETWEEN '" + desde + "' AND '" + hasta + "')");

    return internalSelect(aTable, query);
}

Result<std::vector<DataBaseUtils::KeyAndValue>>
DataBaseUtils::internalSelect(const QString& aTable, const QString& aSelectQuery)
{
    std::vector<DataBaseUtils::KeyAndValue> returnData;

    QSqlQuery query;
    query.exec(aSelectQuery);
    if (aTable == TableNames::BOBINAS) {
        const int idIndex = query.record().indexOf(BobinasFields::ID);
        const int medidaIndex = query.record().indexOf(BobinasFields::MEDIDA);
        const int pesoIndex = query.record().indexOf(BobinasFields::PESO);
        while (query.next()) {
            if (idIndex >= 0) returnData.emplace_back(KeyAndValue(BobinasFields::ID, query.value(idIndex).toString()));
            if (medidaIndex >= 0) returnData.emplace_back(KeyAndValue(BobinasFields::MEDIDA, query.value(medidaIndex).toString()));
            if (pesoIndex >= 0) returnData.emplace_back(KeyAndValue(BobinasFields::PESO, query.value(pesoIndex).toString()));
        }
    } else if (aTable == TableNames::MAQUINAS) {
        const int descripcionIndex = query.record().indexOf(MaquinasFields::DESCRIPCION);
        const int idIndex = query.record().indexOf(MaquinasFields::ID);
        const int idNumero = query.record().indexOf(MaquinasFields::NUMERO);
        const int tipoIndex = query.record().indexOf(MaquinasFields::TIPO);
        while (query.next()) {
            if (descripcionIndex >= 0) returnData.emplace_back(KeyAndValue(MaquinasFields::DESCRIPCION, query.value(descripcionIndex).toString()));
            if (idIndex >= 0) returnData.emplace_back(KeyAndValue(MaquinasFields::ID, query.value(idIndex).toString()));
            if (idNumero >= 0) returnData.emplace_back(KeyAndValue(MaquinasFields::NUMERO, query.value(idIndex).toString()));
            if (tipoIndex >= 0) returnData.emplace_back(KeyAndValue(MaquinasFields::TIPO, query.value(tipoIndex).toString()));
        }
    } else if (aTable == TableNames::MEDIDAS_BOBINAS) {
        const int anchoIndex = query.record().indexOf(MedidasBobinasFields::ANCHO);
        const int idIndex = query.record().indexOf(MedidasBobinasFields::ID);
        const int micronajeIndex = query.record().indexOf(MedidasBobinasFields::MICRONAJE);
        while (query.next()) {
            if (anchoIndex >= 0) returnData.emplace_back(KeyAndValue(MedidasBobinasFields::ANCHO, query.value(anchoIndex).toString()));
            if (idIndex >= 0) returnData.emplace_back(KeyAndValue(MedidasBobinasFields::ID, query.value(idIndex).toString()));
            if (micronajeIndex >= 0) returnData.emplace_back(KeyAndValue(MedidasBobinasFields::MICRONAJE, query.value(micronajeIndex).toString()));
        }
    } else if (aTable == TableNames::MEDIDAS_BOLSAS) {
        const int idIndex = query.record().indexOf(MedidasBolsasFields::ID);
        const int largoIndex = query.record().indexOf(MedidasBolsasFields::LARGO);
        while (query.next()) {
            if (idIndex >= 0) returnData.emplace_back(KeyAndValue(MedidasBolsasFields::ID, query.value(idIndex).toString()));
            if (largoIndex >= 0) returnData.emplace_back(KeyAndValue(MedidasBolsasFields::LARGO, query.value(largoIndex).toString()));
        }
    } else if (aTable == TableNames::OPERARIOS) {
        const int dniIndex = query.record().indexOf(OperariosFields::DNI);
        const int idIndex = query.record().indexOf(OperariosFields::ID);
        const int nombreCompletoIndex = query.record().indexOf(OperariosFields::NOMBRE_COMPLETO);
        while (query.next()) {
            if (dniIndex >= 0) returnData.emplace_back(KeyAndValue(OperariosFields::DNI, query.value(dniIndex).toString()));
            if (idIndex >= 0) returnData.emplace_back(KeyAndValue(OperariosFields::ID, query.value(idIndex).toString()));
            if (nombreCompletoIndex >= 0) returnData.emplace_back(KeyAndValue(OperariosFields::NOMBRE_COMPLETO, query.value(nombreCompletoIndex).toString()));
        }
    } else if (aTable == TableNames::PRODUCTOS_REBOBINADO) {
        const int descripcionIndex = query.record().indexOf(ProductosRebobinadoFields::DESCRIPCION);
        const int idIndex = query.record().indexOf(ProductosRebobinadoFields::ID);
        while (query.next()) {
            if (descripcionIndex >= 0) returnData.emplace_back(KeyAndValue(ProductosRebobinadoFields::DESCRIPCION, query.value(descripcionIndex).toString()));
            if (idIndex >= 0) returnData.emplace_back(KeyAndValue(ProductosRebobinadoFields::ID, query.value(idIndex).toString()));
        }
    } else if (aTable == TableNames::TAREAS_CORTADO) {
        const int cantidadIndex = query.record().indexOf(TareaCortadoFields::CANTIDAD);
        const int fechaIndex = query.record().indexOf(TareaCortadoFields::FECHA);
        const int idIndex = query.record().indexOf(TareaCortadoFields::ID);
        const int idBobinaIndex = query.record().indexOf(TareaCortadoFields::ID_BOBINA);
        const int idMaquinaIndex = query.record().indexOf(TareaCortadoFields::ID_MAQUINA);
        const int idMedidaBolsaIndex = query.record().indexOf(TareaCortadoFields::ID_MEDIDA_BOLSA);
        const int idOperarioIndex = query.record().indexOf(TareaCortadoFields::ID_OPERARIO);
        while (query.next()) {
            if (cantidadIndex >= 0) returnData.emplace_back(KeyAndValue(TareaCortadoFields::CANTIDAD, query.value(cantidadIndex).toString()));
            if (fechaIndex >= 0) returnData.emplace_back(KeyAndValue(TareaCortadoFields::FECHA, query.value(fechaIndex).toString()));
            if (idIndex >= 0) returnData.emplace_back(KeyAndValue(TareaCortadoFields::ID, query.value(idIndex).toString()));
            if (idBobinaIndex >= 0) returnData.emplace_back(KeyAndValue(TareaCortadoFields::ID_BOBINA, query.value(idBobinaIndex).toString()));
            if (idMaquinaIndex >= 0) returnData.emplace_back(KeyAndValue(TareaCortadoFields::ID_MAQUINA, query.value(idMaquinaIndex).toString()));
            if (idMedidaBolsaIndex >= 0) returnData.emplace_back(KeyAndValue(TareaCortadoFields::ID_MEDIDA_BOLSA, query.value(idMedidaBolsaIndex).toString()));
            if (idOperarioIndex >= 0) returnData.emplace_back(KeyAndValue(TareaCortadoFields::ID_OPERARIO, query.value(idOperarioIndex).toString()));
        }
    } else if (aTable == TableNames::TAREAS_EXTRUSADO) {
        const int fechaIndex = query.record().indexOf(TareaExtrusadoFields::FECHA);
        const int idIndex = query.record().indexOf(TareaExtrusadoFields::ID);
        const int idBobinaIndex = query.record().indexOf(TareaExtrusadoFields::ID_BOBINA);
        const int idMaquinaIndex = query.record().indexOf(TareaExtrusadoFields::ID_MAQUINA);
        const int idOperarioIndex = query.record().indexOf(TareaExtrusadoFields::ID_OPERARIO);
        while (query.next()) {
            if (fechaIndex >= 0) returnData.emplace_back(KeyAndValue(TareaExtrusadoFields::FECHA, query.value(fechaIndex).toString()));
            if (idIndex >= 0) returnData.emplace_back(KeyAndValue(TareaExtrusadoFields::ID, query.value(idIndex).toString()));
            if (idBobinaIndex >= 0) returnData.emplace_back(KeyAndValue(TareaExtrusadoFields::ID_BOBINA, query.value(idBobinaIndex).toString()));
            if (idMaquinaIndex >= 0) returnData.emplace_back(KeyAndValue(TareaExtrusadoFields::ID_MAQUINA, query.value(idMaquinaIndex).toString()));
            if (idOperarioIndex >= 0) returnData.emplace_back(KeyAndValue(TareaExtrusadoFields::ID_OPERARIO, query.value(idOperarioIndex).toString()));
        }
    } else if (aTable == TableNames::TAREAS_FILTRADO) {
        const int fechaIndex = query.record().indexOf(TareaFiltradoFields::FECHA);
        const int idIndex = query.record().indexOf(TareaFiltradoFields::ID);
        const int idMaquinaIndex = query.record().indexOf(TareaFiltradoFields::ID_MAQUINA);
        const int idOperarioIndex = query.record().indexOf(TareaFiltradoFields::ID_OPERARIO);
        const int kilosIndex = query.record().indexOf(TareaFiltradoFields::KILOS);
        while (query.next()) {
            if (fechaIndex >= 0) returnData.emplace_back(KeyAndValue(TareaFiltradoFields::FECHA, query.value(fechaIndex).toString()));
            if (idIndex >= 0) returnData.emplace_back(KeyAndValue(TareaFiltradoFields::ID, query.value(idIndex).toString()));
            if (idMaquinaIndex >= 0) returnData.emplace_back(KeyAndValue(TareaFiltradoFields::ID_MAQUINA, query.value(idMaquinaIndex).toString()));
            if (idOperarioIndex >= 0) returnData.emplace_back(KeyAndValue(TareaFiltradoFields::ID_OPERARIO, query.value(idOperarioIndex).toString()));
            if (kilosIndex >= 0) returnData.emplace_back(KeyAndValue(TareaFiltradoFields::KILOS, query.value(kilosIndex).toString()));
        }
    } else if (aTable == TableNames::TAREAS_LAVADO) {
        const int fechaIndex = query.record().indexOf(TareaLavadoFields::FECHA);
        const int idIndex = query.record().indexOf(TareaLavadoFields::ID);
        const int idMaquinaIndex = query.record().indexOf(TareaLavadoFields::ID_MAQUINA);
        const int idOperarioIndex = query.record().indexOf(TareaLavadoFields::ID_OPERARIO);
        const int kilosIndex = query.record().indexOf(TareaLavadoFields::KILOS);
        while (query.next()) {
           if (fechaIndex >= 0) returnData.emplace_back(KeyAndValue(TareaLavadoFields::FECHA, query.value(fechaIndex).toString()));
           if (idIndex >= 0) returnData.emplace_back(KeyAndValue(TareaLavadoFields::ID, query.value(idIndex).toString()));
           if (idMaquinaIndex >= 0) returnData.emplace_back(KeyAndValue(TareaLavadoFields::ID_MAQUINA, query.value(idMaquinaIndex).toString()));
           if (idOperarioIndex >= 0) returnData.emplace_back(KeyAndValue(TareaLavadoFields::ID_OPERARIO, query.value(idOperarioIndex).toString()));
           if (kilosIndex >= 0) returnData.emplace_back(KeyAndValue(TareaLavadoFields::KILOS, query.value(kilosIndex).toString()));
        }
    } else if (aTable == TableNames::TAREAS_REBOBINADO) {
        const int fechaIndex = query.record().indexOf(TareaRebobinadoFields::FECHA);
        const int idIndex = query.record().indexOf(TareaRebobinadoFields::ID);
        const int idMaquinaIndex = query.record().indexOf(TareaRebobinadoFields::ID_MAQUINA);
        const int idOperarioIndex = query.record().indexOf(TareaRebobinadoFields::ID_OPERARIO);
        const int idProductoIndex = query.record().indexOf(TareaRebobinadoFields::ID_PRODUCTO_REBOBINADO);
        const int kilosIndex = query.record().indexOf(TareaRebobinadoFields::KILOS);
        while (query.next()) {
           if (fechaIndex >= 0) returnData.emplace_back(KeyAndValue(TareaRebobinadoFields::FECHA, query.value(fechaIndex).toString()));
           if (idIndex >= 0) returnData.emplace_back(KeyAndValue(TareaRebobinadoFields::ID, query.value(idIndex).toString()));
           if (idProductoIndex >= 0) returnData.emplace_back(KeyAndValue(TareaRebobinadoFields::ID_PRODUCTO_REBOBINADO, query.value(idProductoIndex).toString()));
           if (idMaquinaIndex >= 0) returnData.emplace_back(KeyAndValue(TareaRebobinadoFields::ID_MAQUINA, query.value(idMaquinaIndex).toString()));
           if (idOperarioIndex >= 0) returnData.emplace_back(KeyAndValue(TareaRebobinadoFields::ID_OPERARIO, query.value(idOperarioIndex).toString()));
           if (kilosIndex >= 0) returnData.emplace_back(KeyAndValue(TareaRebobinadoFields::KILOS, query.value(kilosIndex).toString()));
        }
    } else if (aTable == TableNames::TIPOS_MAQUINAS) {
        const int idIndex = query.record().indexOf(TiposMaquinasFields::ID);
        const int tipoIndex = query.record().indexOf(TiposMaquinasFields::TIPO);
        while (query.next()) {
           if (idIndex >= 0) returnData.emplace_back(KeyAndValue(TiposMaquinasFields::ID, query.value(idIndex).toString()));
           if (tipoIndex >= 0)  returnData.emplace_back(KeyAndValue(TiposMaquinasFields::TIPO, query.value(tipoIndex).toString()));
        }
    } else {
        return Result<std::vector<DataBaseUtils::KeyAndValue>>(Status::FAILED, "La tabla no existe");
    }
    return returnData;
}

void
DataBaseUtils::print(const QString aTable, const QString aColumn)
{
    QSqlQuery query("SELECT * FROM " + aTable);
    int idColumn = query.record().indexOf(aColumn);
    while (query.next())
    {
       QString column = query.value(idColumn).toString();
       qDebug() << column;
    }
}

Result<void>
DataBaseUtils::insert(const QString& aTable, const KeyAndValue& aToInsert)
{
    std::vector<KeyAndValue> vec;
    vec.emplace_back(aToInsert);
    return insert(aTable, vec);
}

Result<void>
DataBaseUtils::insert(const QString& aTable, const std::vector<KeyAndValue>& aToInsert)
{
    QString queryStr;
    queryStr.append("INSERT INTO " + aTable + " (");
    for (const KeyAndValue& keyAndValue : aToInsert) {
        queryStr.append(keyAndValue.mKey +  ", ");
    }
    queryStr.chop(2);
    queryStr.append(") ");

    queryStr.append("VALUES (");
    for (const KeyAndValue& keyAndValue : aToInsert) {
        queryStr.append("'" + keyAndValue.mValue + "', ");
    }
    queryStr.chop(2);
    queryStr.append(")");

    QSqlQuery query;
    if(query.exec(queryStr) == false) {
        return Result<void>(Status::FAILED, query.lastError().text());
    }

    return Status::SUCCEEDED;
}

Result<void>
DataBaseUtils::update(const QString& aTable, const std::vector<KeyAndValue>& aToUpdate, const KeyAndValue& aCondition)
{
    std::vector<KeyAndValue> vec;
    vec.emplace_back(aCondition);
    return update(aTable, aToUpdate, vec);
}

Result<void>
DataBaseUtils::update(const QString& aTable, const std::vector<KeyAndValue>& aToUpdate, const std::vector<KeyAndValue>& aConditions)
{
    QString queryS("UPDATE " + aTable + " SET ");
    for (KeyAndValue keyAndValue : aToUpdate) {
        queryS.append(keyAndValue.mKey + " = '" + keyAndValue.mValue + "', ");
    }
    queryS.chop(2);
    queryS.append(" WHERE ");
    for (KeyAndValue keyAndValue : aConditions) {
        queryS.append(keyAndValue.mKey + " = '" + keyAndValue.mValue + "' AND ");
    }
    queryS.chop(5);

    QSqlQuery query;
    if(query.exec(queryS) == false) {
        return Result<void>(Status::FAILED, query.lastError().text());
    }

    return Status::SUCCEEDED;
}

bool
DataBaseUtils::exists(const QString& aTable, const KeyAndValue& aKeysAndValues)
{
    QString queryStr("SELECT * FROM " + aTable + " WHERE (" + aTable + "." + aKeysAndValues.mKey + " = '" + aKeysAndValues.mValue + "')");
    QSqlQuery query(queryStr);
    if (query.next()) {
        return true;
    } else {
        return false;
    }
}

Result<int>
DataBaseUtils::getTableEntriesCount(const QString& aTable)
{
    QSqlQuery query;
    if (query.exec("SELECT COUNT(1) FROM " + aTable) && query.seek(0)) {
        return query.value(0).toInt();
    } else {
        return Result<int>(Status::FAILED, query.lastError().text());
    }

}

Result<QString>
DataBaseUtils::getGratestBobinaId()
{
    std::vector<KeyAndValue> conditions;
    Result<std::vector<KeyAndValue>> result = select(TableNames::BOBINAS, BobinasFields::ID, conditions);
    if (result.status() != Status::SUCCEEDED) {
        return Result<QString>(Status::FAILED, result.error());
    }
    if (result.value().empty()) {
        return QString::number(0);
    } else {
        return result.value().back().mValue;
    }
}

Result<QString>
DataBaseUtils::getCurrentUserId(const int aCurrentUserDNI)
{
    if (exists(TableNames::OPERARIOS,
               KeyAndValue(OperariosFields::DNI, QString::number(aCurrentUserDNI))) == false) {
        return Result<QString>(Status::FAILED, "El operario no existe en la base de datos.");
    }

    std::vector<KeyAndValue> conditions = {KeyAndValue(OperariosFields::DNI, QString::number(aCurrentUserDNI))};
    Result<std::vector<KeyAndValue>> result = select(TableNames::OPERARIOS, OperariosFields::ID, conditions);
    if (result.status() != Status::SUCCEEDED) {
        return Result<QString>(Status::FAILED, result.error());
    }
    if (result.value().size() != 1) {
        return Result<QString>(Status::FAILED, "Error buscando ID de Operario. Posible DNI duplicado?");
    }
    return result.value().begin()->mValue;
}

Result<QString>
DataBaseUtils::getCurrentUserDNI(const QString& aCurrentUserId)
{
    if (exists(TableNames::OPERARIOS,
               KeyAndValue(OperariosFields::ID, aCurrentUserId)) == false) {
        return Result<QString>(Status::FAILED, "El operario no existe en la base de datos.");
    }

    std::vector<KeyAndValue> conditions = {KeyAndValue(OperariosFields::ID, aCurrentUserId)};
    Result<std::vector<KeyAndValue>> result = select(TableNames::OPERARIOS, OperariosFields::DNI, conditions);
    if (result.status() != Status::SUCCEEDED) {
        return Result<QString>(Status::FAILED, result.error());
    }
    if (result.value().size() != 1) {
        return Result<QString>(Status::FAILED, "Error buscando DNI de Operario. Posible ID incorrecto?");
    }
    return result.value().begin()->mValue;
}

Result<QString>
DataBaseUtils::getCurrentUserName(const QString& aCurrentUserDNI)
{
    if (exists(TableNames::OPERARIOS,
               KeyAndValue(OperariosFields::DNI, aCurrentUserDNI)) == false) {
        return Result<QString>(Status::FAILED, "El operario no existe en la base de datos.");
    }

    std::vector<KeyAndValue> conditions = {KeyAndValue(OperariosFields::DNI, aCurrentUserDNI)};
    Result<std::vector<KeyAndValue>> result = select(TableNames::OPERARIOS, OperariosFields::NOMBRE_COMPLETO, conditions);
    if (result.status() != Status::SUCCEEDED) {
        return Result<QString>(Status::FAILED, result.error());
    }
    if (result.value().size() != 1) {
        return Result<QString>(Status::FAILED, "Error buscando Nombre de Operario. Posible DNI duplicado?");
    }
    return result.value().begin()->mValue;
}

Result<QString>
DataBaseUtils::getCurrentMaquinaId(const int aTipoDeMaquina, const int aNumeroDeMaquina)
{
    if (aNumeroDeMaquina == 0) {
        return Result<QString>(Status::FAILED, "Numero de Maquina no seleccionado.");
    }
    std::vector<KeyAndValue> conditions = {KeyAndValue(MaquinasFields::TIPO, QString::number(aTipoDeMaquina)),
                                           KeyAndValue(MaquinasFields::NUMERO, QString::number(aNumeroDeMaquina))};
    Result<std::vector<KeyAndValue>> selectResult = select(TableNames::MAQUINAS, MaquinasFields::ID, conditions);
    if (selectResult.status() != Status::SUCCEEDED) {
        return Result<QString>(Status::FAILED, selectResult.error());
    }
    if (selectResult.value().size() != 1) {
        return Result<QString>(Status::FAILED, "Error buscando ID de Maquina. Posible Maquina duplicada?");
    }
    return selectResult.value().begin()->mValue;
}

QString
DataBaseUtils::getMedidaBobina(const QString& aBobinaId)
{
    QString ancho_micron;
    Result<std::vector<KeyAndValue>> selectResult = select(TableNames::BOBINAS, BobinasFields::MEDIDA, {KeyAndValue(BobinasFields::ID, aBobinaId)});
    const QString idMedida = selectResult.value().begin()->mValue;
    selectResult = select(TableNames::MEDIDAS_BOBINAS, MedidasBobinasFields::ANCHO, {KeyAndValue(MedidasBobinasFields::ID, idMedida)});
    ancho_micron.append(selectResult.value().begin()->mValue).append("/");
    selectResult = select(TableNames::MEDIDAS_BOBINAS, MedidasBobinasFields::MICRONAJE, {KeyAndValue(MedidasBobinasFields::ID, idMedida)});
    ancho_micron.append(selectResult.value().begin()->mValue);
    return ancho_micron;
}

QString
DataBaseUtils::getKilosBobina(const QString& aBobinaId)
{
    Result<std::vector<KeyAndValue>> selectResult = select(TableNames::BOBINAS, BobinasFields::PESO, {KeyAndValue(BobinasFields::ID, aBobinaId)});
    return selectResult.value().back().mValue;
}

QString
DataBaseUtils::getMedidaBolsa(const QString& aMedidaBolsaId)
{
    Result<std::vector<KeyAndValue>> selectResult = select(TableNames::MEDIDAS_BOLSAS, MedidasBolsasFields::LARGO, {KeyAndValue(MedidasBolsasFields::ID, aMedidaBolsaId)});
    return selectResult.value().begin()->mValue;
}

QString
DataBaseUtils::getProductoRebobinadoId(const QString& aDescripcion)
{
    Result<std::vector<KeyAndValue>> selectResult = select(TableNames::PRODUCTOS_REBOBINADO, ProductosRebobinadoFields::ID, {KeyAndValue(ProductosRebobinadoFields::DESCRIPCION, aDescripcion)});
    return selectResult.value().begin()->mValue;
}
