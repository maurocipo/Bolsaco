#include "DataBaseUtils.h"

#include <iostream>

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include "NotificationSender.h"

const char* DataBaseUtils::TableNames::BOBINAS = "Bobinas";
const char* DataBaseUtils::TableNames::MAQUINAS = "Maquinas";
const char* DataBaseUtils::TableNames::MEDIDAS_BOBINAS = "Medidas_Bobinas";
const char* DataBaseUtils::TableNames::MEDIDAS_BOLSAS = "Medidas_Bolsas";
const char* DataBaseUtils::TableNames::OPERARIOS = "Operarios";
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
const char* DataBaseUtils::MaquinasFields::TIPO = DataBaseUtils::TiposMaquinasFields::ID;

const char* DataBaseUtils::MedidasBobinasFields::DESCRIPCION = "Descripcion";
const char* DataBaseUtils::MedidasBobinasFields::ID = "Id_Medida_Bobina";

const char* DataBaseUtils::MedidasBolsasFields::DESCRIPCION = "Descripcion";
const char* DataBaseUtils::MedidasBolsasFields::ID = "Id_Medida_Bolsa";

const char* DataBaseUtils::OperariosFields::DNI = "DNI";
const char* DataBaseUtils::OperariosFields::ID = "Id_Operario";
const char* DataBaseUtils::OperariosFields::NOMBRE_COMPLETO = "Nombre_Completo";

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
const char* DataBaseUtils::TareaRebobinadoFields::ID_BOBINA = DataBaseUtils::BobinasFields::ID;
const char* DataBaseUtils::TareaRebobinadoFields::ID_MAQUINA = DataBaseUtils::MaquinasFields::ID;
const char* DataBaseUtils::TareaRebobinadoFields::ID_OPERARIO = DataBaseUtils::OperariosFields::ID;

const char* DataBaseUtils::TiposMaquinasFields::ID = "Id_Tipo_de_Maquina";
const char* DataBaseUtils::TiposMaquinasFields::TIPO = "Tipo";

const std::string createBobinasStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::BOBINAS) +
                                     "(" +
                                     std::string(DataBaseUtils::BobinasFields::ID)     + " int    NOT NULL," +
                                     std::string(DataBaseUtils::BobinasFields::MEDIDA) + " string NOT NULL," +
                                     std::string(DataBaseUtils::BobinasFields::PESO)   + " float  NOT NULL," +
                                     "PRIMARY KEY (" + std::string(DataBaseUtils::BobinasFields::ID) + ")" +
                                     ");";
const char* DataBaseUtils::CreationCommands::createBobinas = createBobinasStr.c_str();

const std::string createMaquinasStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::MAQUINAS) +
                                      "(" +
                                      std::string(DataBaseUtils::MaquinasFields::DESCRIPCION) + " string         ," +
                                      std::string(DataBaseUtils::MaquinasFields::ID)          + " int    NOT NULL," +
                                      std::string(DataBaseUtils::MaquinasFields::TIPO)        + " int    NOT NULL," +
                                      "PRIMARY KEY (" + std::string(DataBaseUtils::MaquinasFields::ID) + ")" +
                                      ");";
const char* DataBaseUtils::CreationCommands::createMaquinas = createMaquinasStr.c_str();

const std::string createMedidasBobinasStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::MEDIDAS_BOBINAS) +
                                            "(" +
                                            std::string(DataBaseUtils::MedidasBobinasFields::DESCRIPCION) + " string NOT NULL," +
                                            std::string(DataBaseUtils::MedidasBobinasFields::ID)          + " int    NOT NULL," +
                                            "PRIMARY KEY (" + std::string(DataBaseUtils::MedidasBobinasFields::ID) + ")" +
                                            ");";
const char* DataBaseUtils::CreationCommands::createMedidasBobinas = createMedidasBobinasStr.c_str();

const std::string createMedidasBolsasStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::MEDIDAS_BOLSAS) +
                                           "(" +
                                           std::string(DataBaseUtils::MedidasBolsasFields::DESCRIPCION) + " string NOT NULL," +
                                           std::string(DataBaseUtils::MedidasBolsasFields::ID)          + " int    NOT NULL," +
                                           "PRIMARY KEY (" + std::string(DataBaseUtils::MedidasBolsasFields::ID) + ")" +
                                           ");";
const char* DataBaseUtils::CreationCommands::createMedidasBolsas = createMedidasBolsasStr.c_str();

const std::string createOperariosStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::OPERARIOS) +
                                       "(" +
                                       std::string(DataBaseUtils::OperariosFields::ID)              + " int    NOT NULL," +
                                       std::string(DataBaseUtils::OperariosFields::DNI)             + " int    NOT NULL," +
                                       std::string(DataBaseUtils::OperariosFields::NOMBRE_COMPLETO) + " string NOT NULL," +
                                       "PRIMARY KEY (" + std::string(DataBaseUtils::MedidasBolsasFields::ID) + ")" +
                                       ");";
const char* DataBaseUtils::CreationCommands::createOperarios = createOperariosStr.c_str();

const std::string createTareasCortadoStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::TAREAS_CORTADO) +
                                           "(" +
                                           std::string(DataBaseUtils::TareaCortadoFields::CANTIDAD)        + " int  NOT NULL," +
                                           std::string(DataBaseUtils::TareaCortadoFields::FECHA)           + " date NOT NULL," +
                                           std::string(DataBaseUtils::TareaCortadoFields::ID)              + " int  NOT NULL," +
                                           std::string(DataBaseUtils::TareaCortadoFields::ID_BOBINA)       + " int  NOT NULL," +
                                           std::string(DataBaseUtils::TareaCortadoFields::ID_MAQUINA)      + " int  NOT NULL," +
                                           std::string(DataBaseUtils::TareaCortadoFields::ID_MEDIDA_BOLSA) + " int  NOT NULL," +
                                           std::string(DataBaseUtils::TareaCortadoFields::ID_OPERARIO)     + " int  NOT NULL," +
                                           "PRIMARY KEY (" + std::string(DataBaseUtils::TareaCortadoFields::ID) + ")" +
                                           ");";
const char* DataBaseUtils::CreationCommands::createTareasCortado = createTareasCortadoStr.c_str();

const std::string createTareasExtrusadoStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::TAREAS_EXTRUSADO) +
                                             "(" +
                                             std::string(DataBaseUtils::TareaExtrusadoFields::FECHA)       + " date NOT NULL," +
                                             std::string(DataBaseUtils::TareaExtrusadoFields::ID)          + " int  NOT NULL," +
                                             std::string(DataBaseUtils::TareaExtrusadoFields::ID_BOBINA)   + " int  NOT NULL," +
                                             std::string(DataBaseUtils::TareaExtrusadoFields::ID_MAQUINA)  + " int  NOT NULL," +
                                             std::string(DataBaseUtils::TareaExtrusadoFields::ID_OPERARIO) + " int  NOT NULL," +
                                             "PRIMARY KEY (" + std::string(DataBaseUtils::TareaExtrusadoFields::ID) + ")" +
                                             ");";
const char* DataBaseUtils::CreationCommands::createTareasExtrusado = createTareasExtrusadoStr.c_str();

const std::string createTareasFiltradoStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::TAREAS_FILTRADO) +
                                            "(" +
                                            std::string(DataBaseUtils::TareaFiltradoFields::FECHA)       + " date NOT NULL," +
                                            std::string(DataBaseUtils::TareaFiltradoFields::ID)          + " int  NOT NULL," +
                                            std::string(DataBaseUtils::TareaFiltradoFields::ID_MAQUINA)  + " int  NOT NULL," +
                                            std::string(DataBaseUtils::TareaFiltradoFields::ID_OPERARIO) + " int  NOT NULL," +
                                            std::string(DataBaseUtils::TareaFiltradoFields::KILOS)       + " int  NOT NULL," +
                                            "PRIMARY KEY (" + std::string(DataBaseUtils::TareaFiltradoFields::ID) + ")" +
                                            ");";
const char* DataBaseUtils::CreationCommands::createTareasFiltrado = createTareasFiltradoStr.c_str();

const std::string createTareasLavadoStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::TAREAS_LAVADO) +
                                          "(" +
                                          std::string(DataBaseUtils::TareaLavadoFields::FECHA)       + " date NOT NULL," +
                                          std::string(DataBaseUtils::TareaLavadoFields::ID)          + " int  NOT NULL," +
                                          std::string(DataBaseUtils::TareaLavadoFields::ID_MAQUINA)  + " int  NOT NULL," +
                                          std::string(DataBaseUtils::TareaLavadoFields::ID_OPERARIO) + " int  NOT NULL," +
                                          std::string(DataBaseUtils::TareaLavadoFields::KILOS)       + " int  NOT NULL," +
                                          "PRIMARY KEY (" + std::string(DataBaseUtils::TareaLavadoFields::ID) + ")" +
                                          ");";
const char* DataBaseUtils::CreationCommands::createTareasLavado = createTareasLavadoStr.c_str();

const std::string createTareasRebobinadoStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::TAREAS_REBOBINADO) +
                                              "(" +
                                              std::string(DataBaseUtils::TareaRebobinadoFields::FECHA)       + " date NOT NULL," +
                                              std::string(DataBaseUtils::TareaRebobinadoFields::ID)          + " int  NOT NULL," +
                                              std::string(DataBaseUtils::TareaRebobinadoFields::ID_BOBINA)   + " int  NOT NULL," +
                                              std::string(DataBaseUtils::TareaRebobinadoFields::ID_MAQUINA)  + " int  NOT NULL," +
                                              std::string(DataBaseUtils::TareaRebobinadoFields::ID_OPERARIO) + " int  NOT NULL," +
                                              "PRIMARY KEY (" + std::string(DataBaseUtils::TareaRebobinadoFields::ID) + ")" +
                                              ");";
const char* DataBaseUtils::CreationCommands::createTareasRebobinado = createTareasRebobinadoStr.c_str();

const std::string createTiposMaquinasStr = "CREATE TABLE " + std::string(DataBaseUtils::TableNames::TIPOS_MAQUINAS) +
                                           "(" +
                                           std::string(DataBaseUtils::TiposMaquinasFields::ID)   + " int     NOT NULL," +
                                           std::string(DataBaseUtils::TiposMaquinasFields::TIPO) + " string  NOT NULL," +
                                           "PRIMARY KEY (" + std::string(DataBaseUtils::TiposMaquinasFields::ID) + ")" +
                                           ");";
const char* DataBaseUtils::CreationCommands::createTiposMaquinas = createTiposMaquinasStr.c_str();

QString
DataBaseUtils::buildSelectQuery(const QString& aTable, const QString& aColumn, const KeyAndValue& aCondition)
{
    std::vector<KeyAndValue> vec;
    vec.emplace_back(aCondition);
    return buildSelectQuery(aTable, aColumn, vec);
}

QString
DataBaseUtils::buildSelectQuery(const QString& aTable, const QString& aColumn, const std::vector<KeyAndValue>& aConditions)
{
    QString queryStr;
    queryStr.append("SELECT " + aColumn + " FROM " + aTable + " WHERE ");
    for(const KeyAndValue& keyAndValue : aConditions) {
        queryStr.append(keyAndValue.mKey + " = '" + keyAndValue.mValue + "' AND ");
    }
    if (aConditions.empty() == false) {
        queryStr.chop(5);
    } else {
        queryStr.chop(7);
    }
    return queryStr;
}

QString
DataBaseUtils::buildOngoingSelectQuery(const QString& aTable, const QString& aColumn, const std::vector<KeyAndValue>& aConditions)
{
    QString queryStr;
    queryStr.append("SELECT " + aColumn + " FROM " + aTable + " WHERE ");
    for(const KeyAndValue& keyAndValue : aConditions) {
        queryStr.append(keyAndValue.mKey + " LIKE '%" + keyAndValue.mValue + "%' AND ");
    }
    if (aConditions.empty() == false) {
        queryStr.chop(5);
    } else {
        queryStr.chop(7);
    }
    return queryStr;
}

Result<std::vector<DataBaseUtils::KeyAndValue>>
DataBaseUtils::select(const QString& aTable, const QString& aColumn, const QString& aExplicitCondition)
{
    QString queryStr = buildSelectQuery(aTable, aColumn, std::vector<KeyAndValue>());
    queryStr.append( " WHERE " + aExplicitCondition);
    return internalSelect(aTable, queryStr);
}

Result<std::vector<DataBaseUtils::KeyAndValue>>
DataBaseUtils::select(const QString& aTable, const QString& aColumn, const KeyAndValue& aCondition)
{
    std::vector<KeyAndValue> vec;
    vec.emplace_back(aCondition);
    return select(aTable, aColumn, vec);
}

Result<std::vector<DataBaseUtils::KeyAndValue>>
DataBaseUtils::select(const QString& aTable, const QString& aColumn, const std::vector<KeyAndValue>& aConditions)
{
    return internalSelect(aTable, buildSelectQuery(aTable, aColumn, aConditions));
}

Result<std::vector<DataBaseUtils::KeyAndValue>>
DataBaseUtils::internalSelect(const QString& aTable, const QString& aSelectQuery)
{
    std::vector<DataBaseUtils::KeyAndValue> returnData;

/*    QSqlQuery query;
    query.exec(aSelectQuery);
    if (aTable == TableNames::MAQUINA) {
        int descripcionIndex = query.record().indexOf(MaquinaFields::DESCRIPCION);
        int idIndex = query.record().indexOf(MaquinaFields::ID);
        int tipoIndex = query.record().indexOf(MaquinaFields::TIPO);
        while (query.next()) {
            returnData.emplace_back(KeyAndValue(MaquinaFields::DESCRIPCION, query.value(descripcionIndex).toString()));
            returnData.emplace_back(KeyAndValue(MaquinaFields::ID, query.value(idIndex).toString()));
            returnData.emplace_back(KeyAndValue(MaquinaFields::TIPO, query.value(tipoIndex).toString()));
        }
    } else if (aTable == TableNames::OPERADOR) {
        int apellidoIndex = query.record().indexOf(OperadorFields::APELLIDO);
        int dniIndex = query.record().indexOf(OperadorFields::DNI);
        int nombreIndex = query.record().indexOf(OperadorFields::NOMBRE);
        while (query.next()) {
            returnData.emplace_back(KeyAndValue(OperadorFields::APELLIDO, query.value(apellidoIndex).toString()));
            returnData.emplace_back(KeyAndValue(OperadorFields::DNI, query.value(dniIndex).toString()));
            returnData.emplace_back(KeyAndValue(OperadorFields::NOMBRE, query.value(nombreIndex).toString()));
        }
    } else if (aTable == TableNames::TAREA) {
        int dniIndex = query.record().indexOf(TareaFields::DNI_OPERADOR);
        int fechaIndex = query.record().indexOf(TareaFields::FECHA);
        int idIndex = query.record().indexOf(TareaFields::ID);
        int bobinaIndex = query.record().indexOf(TareaFields::ID_BOBINA);
        int bolsaIndex = query.record().indexOf(TareaFields::ID_BOLSA);
        int maquinaIndex = query.record().indexOf(TareaFields::ID_MAQUINA);
        int resultadoIndex = query.record().indexOf(TareaFields::RESULTADO);
        while (query.next()) {
            returnData.emplace_back(KeyAndValue(TareaFields::DNI_OPERADOR, query.value(dniIndex).toString()));
            returnData.emplace_back(KeyAndValue(TareaFields::FECHA, query.value(fechaIndex).toString()));
            returnData.emplace_back(KeyAndValue(TareaFields::ID, query.value(idIndex).toString()));
            returnData.emplace_back(KeyAndValue(TareaFields::ID_BOBINA, query.value(bobinaIndex).toString()));
            returnData.emplace_back(KeyAndValue(TareaFields::ID_BOLSA, query.value(bolsaIndex).toString()));
            returnData.emplace_back(KeyAndValue(TareaFields::ID_MAQUINA, query.value(maquinaIndex).toString()));
            returnData.emplace_back(KeyAndValue(TareaFields::RESULTADO, query.value(resultadoIndex).toString()));
        }
    } else {
        return Result<std::vector<DataBaseUtils::KeyAndValue>>(Status::FAILED, "La tabla no existe");
    }*/
    return returnData;
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

    queryStr.append("VALUES(");
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

QString
DataBaseUtils::convertDateToDDMMYYY(const QDate& aDate)
{
    return aDate.toString("dd-MM-yyyy");
}

QString
DataBaseUtils::convertDateToYYYYMMDD(const QDate& aDate)
{
    return aDate.toString("yyyy-MM-dd");
}
