#include "Lavadora.h"
#include "ui_Lavadora.h"

Lavadora::Lavadora(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Lavadora)
{
    ui->setupUi(this);
}

Lavadora::~Lavadora()
{
    delete ui;
}
