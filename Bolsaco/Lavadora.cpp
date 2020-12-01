#include "Lavadora.h"
#include "ui_Lavadora.h"

Lavadora::Lavadora(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::Lavadora)
{
    mUi->setupUi(this);
}

Lavadora::~Lavadora()
{
    delete mUi;
}

std::vector<double>
Lavadora::getKilosBolsones() const
{
    std::vector<double> returnData;
    if (mUi->doubleSpinBox_1->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_1->value());
    if (mUi->doubleSpinBox_2->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_2->value());
    if (mUi->doubleSpinBox_3->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_3->value());
    if (mUi->doubleSpinBox_4->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_4->value());
    if (mUi->doubleSpinBox_5->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_5->value());
    if (mUi->doubleSpinBox_6->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_6->value());
    if (mUi->doubleSpinBox_7->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_7->value());
    if (mUi->doubleSpinBox_8->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_8->value());
    if (mUi->doubleSpinBox_9->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_9->value());
    if (mUi->doubleSpinBox_10->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_10->value());
    if (mUi->doubleSpinBox_11->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_11->value());
    if (mUi->doubleSpinBox_12->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_12->value());
    if (mUi->doubleSpinBox_13->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_13->value());
    if (mUi->doubleSpinBox_14->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_14->value());
    if (mUi->doubleSpinBox_15->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_15->value());
    if (mUi->doubleSpinBox_16->value() != 0) returnData.emplace_back(mUi->doubleSpinBox_16->value());
    return returnData;
}

void
Lavadora::clear()
{
    mUi->doubleSpinBox_1->clear();
    mUi->doubleSpinBox_2->clear();
    mUi->doubleSpinBox_3->clear();
    mUi->doubleSpinBox_4->clear();
    mUi->doubleSpinBox_5->clear();
    mUi->doubleSpinBox_6->clear();
    mUi->doubleSpinBox_7->clear();
    mUi->doubleSpinBox_8->clear();
    mUi->doubleSpinBox_9->clear();
    mUi->doubleSpinBox_10->clear();
    mUi->doubleSpinBox_11->clear();
    mUi->doubleSpinBox_12->clear();
    mUi->doubleSpinBox_13->clear();
    mUi->doubleSpinBox_14->clear();
    mUi->doubleSpinBox_15->clear();
    mUi->doubleSpinBox_16->clear();
}
