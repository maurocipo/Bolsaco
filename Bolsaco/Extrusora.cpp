#include "Extrusora.h"
#include "ui_Extrusora.h"

Extrusora::Extrusora(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::Extrusora)
{
    mUi->setupUi(this);

    const QStringList medidas = {"Medida1", "Medida2", "Medida3"};
    mUi->comboBox->addItems(medidas);
}

Extrusora::~Extrusora()
{
    delete mUi;
}
