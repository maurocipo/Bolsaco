#ifndef DATABASEDATA_H
#define DATABASEDATA_H

#include "DataBaseUtils.h"

namespace DataBaseData {

const QStringList
MedidasAnchoBobinas {"10",
                     "20"};
const QStringList
MedidasMicronajeBobinas {"30",
                         "40"};

const QStringList
MedidasLargoBolsas {"40",
                    "50",
                    "60"};

const QStringList
ProductosRebobinadoStr {"Film 50cm c/ Mango",
                        "Film 50cm Boca Ancha",
                        "Film 50cm Automatico",
                        "Film 10cm"};

enum ProductosRebobinados
{
    FILM50CMANGO = 1,
    FILM50BOCAANCHA,
    FILM50AUTOMATICO,
    FILM10
};

const std::vector<DataBaseUtils::Operario>
Operarios {
    DataBaseUtils::Operario("Mauro Cipolatti", 34990189),
    DataBaseUtils::Operario("Martin Cipolatti", 28268919),
    DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
            DataBaseUtils::Operario("Juan Perez", 10000000),
};

// This enum matches with CantidadMaquinas array.
enum TiposMaquinas
{
    INVALID = -1,
    CORTADORA,
    EXTRUSORA,
    FILTRADORA,
    LAVADORA,
    REBOBINADORA
};

const QStringList
TiposMaquinasStr
{
    "Cortadora",
    "Filmera", // = Extrusora
    "Filtradora",
    "Lavadora",
    "Rebobinadora"
};

// Numbers in this array match with TiposMaquinas enum order.
const int
CantidadMaquinas[] {
    4, // Cortadoras
    6, // Extrusoras
    2, // Filtradoras
    1, // Lavadora
    1, // Rebobinadora
};

const std::vector<DataBaseUtils::Maquina>
Maquinas {
    DataBaseUtils::Maquina(TiposMaquinas::CORTADORA, 1, "Ej: verde"),
    DataBaseUtils::Maquina(TiposMaquinas::CORTADORA, 2, "Ej: verde"),
    DataBaseUtils::Maquina(TiposMaquinas::CORTADORA, 3, "Ej: verde"),
    DataBaseUtils::Maquina(TiposMaquinas::CORTADORA, 4, "Ej: verde"),
    DataBaseUtils::Maquina(TiposMaquinas::EXTRUSORA, 1, "Ej: azul"),
    DataBaseUtils::Maquina(TiposMaquinas::EXTRUSORA, 2, "Ej: azul"),
    DataBaseUtils::Maquina(TiposMaquinas::EXTRUSORA, 3, "Ej: azul"),
    DataBaseUtils::Maquina(TiposMaquinas::EXTRUSORA, 4, "Ej: azul"),
    DataBaseUtils::Maquina(TiposMaquinas::EXTRUSORA, 5, "Ej: azul"),
    DataBaseUtils::Maquina(TiposMaquinas::EXTRUSORA, 6, "Ej: azul"),
    DataBaseUtils::Maquina(TiposMaquinas::FILTRADORA, 1, "Ej: azul"),
    DataBaseUtils::Maquina(TiposMaquinas::FILTRADORA, 2, "Ej: azul"),
    DataBaseUtils::Maquina(TiposMaquinas::LAVADORA, 1, "Ej: azul"),
    DataBaseUtils::Maquina(TiposMaquinas::REBOBINADORA, 1, "Ej: azul")
};

}
#endif // DATABASEDATA_H
