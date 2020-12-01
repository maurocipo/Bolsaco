#include "TrazabilidadPorBobinaOptions.h"
#include "ui_TrazabilidadPorBobinaOptions.h"

TrazabilidadPorBobinaOptions::TrazabilidadPorBobinaOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrazabilidadPorBobinaOptions)
{
    ui->setupUi(this);
}

TrazabilidadPorBobinaOptions::~TrazabilidadPorBobinaOptions()
{
    delete ui;
}
