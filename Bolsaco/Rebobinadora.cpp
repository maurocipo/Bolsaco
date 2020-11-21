#include "Rebobinadora.h"
#include "ui_Rebobinadora.h"

Rebobinadora::Rebobinadora(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Rebobinadora)
{
    ui->setupUi(this);
}

Rebobinadora::~Rebobinadora()
{
    delete ui;
}
