#include "ProduccionPorMaquinaOptions.h"
#include "ui_ProduccionPorMaquinaOptions.h"

ProduccionPorMaquinaOptions::ProduccionPorMaquinaOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProduccionPorMaquinaOptions)
{
    ui->setupUi(this);
}

ProduccionPorMaquinaOptions::~ProduccionPorMaquinaOptions()
{
    delete ui;
}
