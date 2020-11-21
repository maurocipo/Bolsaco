#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QDate>
#include <QString>
#include <sstream>
#include <vector>

#include "Result.h"

namespace DataBaseUtils
{
    struct TableNames
    {
        static const char* BOBINAS;
        static const char* MAQUINAS;
        static const char* MEDIDAS_BOBINAS;
        static const char* MEDIDAS_BOLSAS;
        static const char* OPERARIOS;
        static const char* TAREAS_CORTADO;
        static const char* TAREAS_EXTRUSADO;
        static const char* TAREAS_FILTRADO;
        static const char* TAREAS_LAVADO;
        static const char* TAREAS_REBOBINADO;
        static const char* TIPOS_MAQUINAS;
    };
//------------------------------------------------------
    struct CreationCommands
    {
        static const char* createBobinas;
        static const char* createMaquinas;
        static const char* createMedidasBobinas;
        static const char* createMedidasBolsas;
        static const char* createOperarios;
        static const char* createTareasCortado;
        static const char* createTareasExtrusado;
        static const char* createTareasFiltrado;
        static const char* createTareasLavado;
        static const char* createTareasRebobinado;
        static const char* createTiposMaquinas
    };
//------------------------------------------------------
    struct BobinasFields
    {
        static const char* ID;
        static const char* MEDIDA;
        static const char* PESO;
    };
    struct MaquinasFields
    {
        static const char* ID;
        static const char* TIPO;
        static const char* DESCRIPCION;
    };
    struct MedidasBobinasFields
    {
        static const char* ID;
        static const char* DESCRIPCION;
    };
    struct MedidasBolsasFields
    {
        static const char* ID;
        static const char* DESCRIPCION;
    };
    struct OperariosFields
    {
        static const char* ID;
        static const char* NOMBRE_COMPLETO;
        static const char* DNI;
    };
    struct TareaCortadoFields
    {
        static const char* ID;
        static const char* ID_OPERARIO;
        static const char* ID_MAQUINA;
        static const char* ID_BOBINA;
        static const char* ID_MEDIDA_BOLSA;
        static const char* CANTIDAD;
        static const char* FECHA;
    };
    struct TareaExtrusadoFields
    {
        static const char* ID;
        static const char* ID_OPERARIO;
        static const char* ID_MAQUINA;
        static const char* ID_BOBINA;
        static const char* FECHA;
    };
    struct TareaFiltradoFields
    {
        static const char* ID;
        static const char* ID_OPERARIO;
        static const char* ID_MAQUINA;
        static const char* KILOS;
        static const char* FECHA;
    };
    struct TareaLavadoFields
    {
        static const char* ID;
        static const char* ID_OPERARIO;
        static const char* ID_MAQUINA;
        static const char* KILOS;
        static const char* FECHA;
    };
    struct TareaRebobinadoFields
    {
        static const char* ID;
        static const char* ID_OPERARIO;
        static const char* ID_MAQUINA;
        static const char* ID_BOBINA;
        static const char* FECHA;
    };
    struct TiposMaquinasFields
    {
        static const char* ID;
        static const char* TIPO;
    };
//------------------------------------------------------
    enum MedidasBobinas
    {
        MEDIDA11,
        MEDIDA22
    };
    enum MedidasBolsas
    {
        MEDIDA1,
        MEDIDA2,
        MEDIDA3,
        MEDIDA4,
        MEDIDA5
    };
    enum TiposMaquinas
    {
        INVALID = -1,
        CORTADORA,
        EXTRUSORA,
        FILTRADORA,
        LAVADORA,
        REBOBINADORA
    };
//------------------------------------------------------
    struct KeyAndValue
    {
        KeyAndValue(const QString& aKey, const QString& aValue)
            : mKey(aKey), mValue(aValue) {}

        QString mKey;
        QString mValue;
    };

    QString
    buildSelectQuery(const QString& aTable, const QString& aColumn, const KeyAndValue& aCondition);
    QString
    buildSelectQuery(const QString& aTable, const QString& aColumn, const std::vector<KeyAndValue>& aConditions);
    QString
    buildOngoingSelectQuery(const QString& aTable, const QString& aColumn, const std::vector<KeyAndValue>& aConditions);

    Result<std::vector<KeyAndValue>>
    select(const QString& aTable, const QString& aColumn, const QString& aExplicitCondition);
    Result<std::vector<KeyAndValue>>
    select(const QString& aTable, const QString& aColumn, const KeyAndValue& aCondition);
    Result<std::vector<KeyAndValue>>
    select(const QString& aTable, const QString& aColumn, const std::vector<KeyAndValue>& aConditions);
    Result<std::vector<KeyAndValue>>
    internalSelect(const QString& aTable, const QString& aSelectQuery);

    Result<void>
    insert(const QString& aTable, const std::vector<KeyAndValue>& aToInsert);

    Result<void>
    update(const QString& aTable, const std::vector<KeyAndValue>& aToUpdate, const KeyAndValue& aCondition);
    Result<void>
    update(const QString& aTable, const std::vector<KeyAndValue>& aToUpdate, const std::vector<KeyAndValue>& aConditions);

    bool
    exists(const QString& aTable, const KeyAndValue& aKeyAndValue);

    Result<int>
    getTableEntriesCount(const QString& aTable);

    QString
    convertDateToDDMMYYY(const QDate& aDate);
    QString
    convertDateToYYYYMMDD(const QDate& aDate);
};
#endif // DATABASEUTILS_H
