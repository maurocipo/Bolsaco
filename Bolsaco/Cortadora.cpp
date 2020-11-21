#include "Cortadora.h"
#include "ui_Cortadora.h"

Cortadora::Cortadora(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cortadora)
{
    ui->setupUi(this);
}

Cortadora::~Cortadora()
{
    delete ui;
}

std::vector<DataBaseUtils::KeyAndValue>
Cortadora::getData() const
{
    std::vector<DataBaseUtils::KeyAndValue> data;
/*    double value = ui->spinBox_Medida1->value();
    if (value != 0) {
        data.push_back(DataBaseUtils::KeyAndValue(DataBaseUtils::TareaFields::ID_BOLSA, QString(DataBaseUtils::Bolsas::MEDIDA1)));
        data.push_back(DataBaseUtils::KeyAndValue(DataBaseUtils::TareaFields::RESULTADO, QString::number(value)));
    }
    value = ui->spinBox_Medida2->value();
    if (value != 0) {
        data.push_back(DataBaseUtils::KeyAndValue(DataBaseUtils::TareaFields::ID_BOLSA, QString(DataBaseUtils::Bolsas::MEDIDA2)));
        data.push_back(DataBaseUtils::KeyAndValue(DataBaseUtils::TareaFields::RESULTADO, QString::number(value)));
    }
    value = ui->spinBox_Medida3->value();
    if (value != 0) {
        data.push_back(DataBaseUtils::KeyAndValue(DataBaseUtils::TareaFields::ID_BOLSA, QString(DataBaseUtils::Bolsas::MEDIDA3)));
        data.push_back(DataBaseUtils::KeyAndValue(DataBaseUtils::TareaFields::RESULTADO, QString::number(value)));
    }
    value = ui->spinBox_Medida4->value();
    if (value != 0) {
        data.push_back(DataBaseUtils::KeyAndValue(DataBaseUtils::TareaFields::ID_BOLSA, QString(DataBaseUtils::Bolsas::MEDIDA4)));
        data.push_back(DataBaseUtils::KeyAndValue(DataBaseUtils::TareaFields::RESULTADO, QString::number(value)));
    }
    value = ui->spinBox_Medida5->value();
    if (value != 0) {
        data.push_back(DataBaseUtils::KeyAndValue(DataBaseUtils::TareaFields::ID_BOLSA, QString(DataBaseUtils::Bolsas::MEDIDA5)));
        data.push_back(DataBaseUtils::KeyAndValue(DataBaseUtils::TareaFields::RESULTADO, QString::number(value)));
    }
*/    return data;
}
