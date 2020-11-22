#ifndef DATABASEDATA_H
#define DATABASEDATA_H

#include "DataBaseUtils.h"

namespace DataBaseData {

const QStringList
MedidasBobinas {"30",
                "40"};

const QStringList
MedidasBolsas {"30x40",
               "40x50"};

const std::vector<DataBaseUtils::Operario>
Operarios {
    DataBaseUtils::Operario("Mauro Cipolatti", 34990189),
    DataBaseUtils::Operario("Martin Cipolatti", 28268919)
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

QStringList TiposMaquinasStr
{
    "Cortadora",
    "Extrusora",
    "Filtradora",
    "Lavadora",
    "Rebobinadora"
};

const std::vector<DataBaseUtils::Maquina>
Maquinas {
    DataBaseUtils::Maquina(TiposMaquinas::CORTADORA, "Ej: verde"),
    DataBaseUtils::Maquina(TiposMaquinas::EXTRUSORA, "Ej: azul")
};

// Numbers in this array match with TiposMaquinas enum order.
const int
CantidadMaquinas[] {
    4, // Cortadora
    6, // Extrusora
    2, // Filtradora
    1, // Lavadora
    1, // Rebobinadora
};
}
#endif // DATABASEDATA_H
