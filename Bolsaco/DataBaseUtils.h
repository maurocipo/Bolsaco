#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QDate>
#include <QString>
#include <sstream>
#include <vector>

#include "Result.h"

namespace DataBaseUtils
{
    extern const char* dateFormat;

    struct TableNames
    {
        static const char* BOBINAS;
        static const char* MAQUINAS;
        static const char* MEDIDAS_BOBINAS;
        static const char* MEDIDAS_BOLSAS;
        static const char* OPERARIOS;
        static const char* PRODUCTOS_REBOBINADO;
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
        static const char* createProductosRebobinado;
        static const char* createTareasCortado;
        static const char* createTareasExtrusado;
        static const char* createTareasFiltrado;
        static const char* createTareasLavado;
        static const char* createTareasRebobinado;
        static const char* createTiposMaquinas;
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
        static const char* NUMERO;
        static const char* DESCRIPCION;
    };
    struct MedidasBobinasFields
    {
        static const char* ID;
        static const char* ANCHO;
        static const char* MICRONAJE;
    };
    struct MedidasBolsasFields
    {
        static const char* ID;
        static const char* LARGO;
    };
    struct OperariosFields
    {
        static const char* ID;
        static const char* NOMBRE_COMPLETO;
        static const char* DNI;
    };
    struct ProductosRebobinadoFields
    {
        static const char* ID;
        static const char* DESCRIPCION;
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
        static const char* ID_PRODUCTO_REBOBINADO;
        static const char* KILOS;
        static const char* FECHA;
    };
    struct TiposMaquinasFields
    {
        static const char* ID;
        static const char* TIPO;
    };
//------------------------------------------------------
    struct Maquina {
        Maquina(const int aTipo, const int aNumero, const QString aDescripcion):
            mTipo(aTipo),
            mNumero(aNumero),
            mDescripcion(aDescripcion) {}

        int mTipo;
        int mNumero;
        QString mDescripcion;
    };

    struct Operario {
        Operario(const QString& aNombreYApellido, const int aDNI):
            mNombreYApellido(aNombreYApellido),
            mDNI(aDNI) {}

        QString mNombreYApellido;
        int mDNI;
    };
//------------------------------------------------------
    struct KeyAndValue
    {
        KeyAndValue() {};
        KeyAndValue(const QString& aKey, const QString& aValue)
            : mKey(aKey), mValue(aValue) {}

        QString mKey;
        QString mValue;
    };

    struct FechaKeyAndValues
    {
        FechaKeyAndValues() {};
        FechaKeyAndValues(const QString& aKey, const QString& aDesde, const QString& aHasta)
            : mKey(aKey), mDesde(aDesde), mHasta(aHasta) {}

        QString mKey;
        QString mDesde;
        QString mHasta;
    };

    QString
    buildSelectQuery(const QString& aTable, const QString& aColumn, const std::vector<KeyAndValue>& aConditions);

    Result<std::vector<KeyAndValue>>
    select(const QString& aTable, const QString& aColumn, const std::vector<KeyAndValue>& aConditions);

    Result<std::vector<KeyAndValue>>
    selectBetweenDates(const QString& aTable, const QString& aColumn, const std::vector<KeyAndValue>& aConditions, const FechaKeyAndValues& aDates);

    Result<std::vector<KeyAndValue>>
    internalSelect(const QString& aTable, const QString& aSelectQuery);

    void print(const QString aTable, const QString aColumn);

    Result<void>
    insert(const QString& aTable, const KeyAndValue& aToInsert);
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

    Result<QString>
    getGratestBobinaId();

    Result<QString>
    getCurrentUserId(const int aCurrentUser);

    Result<QString>
    getCurrentMaquinaId(const int aTipoDeMaquina, const int aNumeroDeMaquina);

};
#endif // DATABASEUTILS_H
