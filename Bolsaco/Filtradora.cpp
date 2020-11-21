#include "Filtradora.h"
#include "ui_Filtradora.h"

Filtradora::Filtradora(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Filtradora)
{
    ui->setupUi(this);
}

Filtradora::~Filtradora()
{
    delete ui;
}
