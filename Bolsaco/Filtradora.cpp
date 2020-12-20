#include "Filtradora.h"
#include "ui_Filtradora.h"

Filtradora::Filtradora(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::Filtradora)
{
    mUi->setupUi(this);
}

Filtradora::~Filtradora()
{
    delete mUi;
}

void
Filtradora::clear()
{
    mUi->doubleSpinBox->clear();
}

double
Filtradora::getKilos() const
{
    return mUi->doubleSpinBox->value();
}

bool
Filtradora::hasUnsavedWork() const {
    if (mUi->doubleSpinBox->value() != 0) {
        return true;
    } else {
        return false;
    }
}
