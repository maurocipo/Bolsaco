#include "Cortadora.h"
#include "ui_Cortadora.h"

Cortadora::Cortadora(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::Cortadora)
{
    mUi->setupUi(this);
}

Cortadora::~Cortadora()
{
    delete mUi;
}

void
Cortadora::clear()
{
    mUi->comboBox_Largo->setCurrentIndex(-1);

    mUi->spinBox_CantBobina1->clear();
    mUi->spinBox_CantBobina2->clear();
    mUi->spinBox_CantBobina3->clear();
    mUi->spinBox_CantBobina4->clear();
    mUi->spinBox_CantBobina5->clear();

    mUi->spinBox_NumBobina1->clear();
    mUi->spinBox_NumBobina2->clear();
    mUi->spinBox_NumBobina3->clear();
    mUi->spinBox_NumBobina4->clear();
    mUi->spinBox_NumBobina5->clear();
}

void
Cortadora::fillLargos(const QStringList& aMedidas)
{
    mUi->comboBox_Largo->clear();
    mUi->comboBox_Largo->addItems(aMedidas);
    mUi->comboBox_Largo->setCurrentIndex(-1);
}

QString
Cortadora::getLargo() const
{
    return mUi->comboBox_Largo->currentText();
}

std::vector<BobinaIdAndCortes>
Cortadora::getData() const
{
    std::vector<BobinaIdAndCortes> returnData;

    if (mUi->spinBox_NumBobina1->value() != 0 && mUi->spinBox_CantBobina1->value() != 0) returnData.emplace_back(BobinaIdAndCortes(QString::number(mUi->spinBox_NumBobina1->value()), QString::number(mUi->spinBox_CantBobina1->value())));
    if (mUi->spinBox_NumBobina2->value() != 0 && mUi->spinBox_CantBobina2->value() != 0) returnData.emplace_back(BobinaIdAndCortes(QString::number(mUi->spinBox_NumBobina2->value()), QString::number(mUi->spinBox_CantBobina2->value())));
    if (mUi->spinBox_NumBobina3->value() != 0 && mUi->spinBox_CantBobina3->value() != 0) returnData.emplace_back(BobinaIdAndCortes(QString::number(mUi->spinBox_NumBobina3->value()), QString::number(mUi->spinBox_CantBobina3->value())));
    if (mUi->spinBox_NumBobina4->value() != 0 && mUi->spinBox_CantBobina4->value() != 0) returnData.emplace_back(BobinaIdAndCortes(QString::number(mUi->spinBox_NumBobina4->value()), QString::number(mUi->spinBox_CantBobina4->value())));
    if (mUi->spinBox_NumBobina5->value() != 0 && mUi->spinBox_CantBobina5->value() != 0) returnData.emplace_back(BobinaIdAndCortes(QString::number(mUi->spinBox_NumBobina5->value()), QString::number(mUi->spinBox_CantBobina5->value())));

    return returnData;
}

bool
Cortadora::hasUnsavedWork() const
{
    if (mUi->spinBox_NumBobina1->value() != 0 || mUi->spinBox_CantBobina1->value() != 0 ||
        mUi->spinBox_NumBobina2->value() != 0 || mUi->spinBox_CantBobina2->value() != 0 ||
        mUi->spinBox_NumBobina3->value() != 0 || mUi->spinBox_CantBobina3->value() != 0 ||
        mUi->spinBox_NumBobina4->value() != 0 || mUi->spinBox_CantBobina4->value() != 0 ||
        mUi->spinBox_NumBobina5->value() != 0 || mUi->spinBox_CantBobina5->value() != 0) {

        return true;
    } else {
        return false;
    }
}
