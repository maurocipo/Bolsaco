#include "Rebobinadora.h"
#include "ui_Rebobinadora.h"

#include "DataBaseData.h"

Rebobinadora::Rebobinadora(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::Rebobinadora)
{
    mUi->setupUi(this);
}

Rebobinadora::~Rebobinadora()
{
    delete mUi;
}

void
Rebobinadora::clear()
{
    mUi->spinBox_Film10->clear();
    mUi->spinBox_Film50BocaAncha->clear();
    mUi->spinBox_Film50Automatico->clear();
    mUi->spinBox_Film50CMango->clear();
}

std::vector<ProductIdAndKilos>
Rebobinadora::getKilos() const
{
    std::vector<ProductIdAndKilos> returnData;
    returnData.emplace_back(ProductIdAndKilos(DataBaseData::ProductosRebobinados::FILM10, mUi->spinBox_Film10->value()));
    returnData.emplace_back(ProductIdAndKilos(DataBaseData::ProductosRebobinados::FILM50AUTOMATICO, mUi->spinBox_Film50Automatico->value()));
    returnData.emplace_back(ProductIdAndKilos(DataBaseData::ProductosRebobinados::FILM50BOCAANCHA, mUi->spinBox_Film50BocaAncha->value()));
    returnData.emplace_back(ProductIdAndKilos(DataBaseData::ProductosRebobinados::FILM50CMANGO, mUi->spinBox_Film50CMango->value()));
    return returnData;

}
