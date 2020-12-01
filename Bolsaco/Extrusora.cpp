#include "Extrusora.h"
#include "ui_Extrusora.h"

#include "DataBaseData.h"

Extrusora::Extrusora(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::Extrusora)
{
    mUi->setupUi(this);
}

Extrusora::~Extrusora()
{
    delete mUi;
}

void
Extrusora::clear()
{
    mUi->comboBox->setCurrentIndex(-1);

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
    mUi->doubleSpinBox_17->clear();
    mUi->doubleSpinBox_18->clear();
    mUi->doubleSpinBox_19->clear();
    mUi->doubleSpinBox_20->clear();
}

void
Extrusora::fillMedidas(const QStringList& aMedidas)
{
    mUi->comboBox->clear();
    mUi->comboBox->addItems(aMedidas);
    mUi->comboBox->setCurrentIndex(-1);
}

void
Extrusora::setBobinasIds(const QString& aCurrentGratestBobinaId)
{
    int bobinaId = aCurrentGratestBobinaId.toInt() + 1;
    mUi->label_Bobina1->setText(QString::number(bobinaId++));
    mUi->label_Bobina2->setText(QString::number(bobinaId++));
    mUi->label_Bobina3->setText(QString::number(bobinaId++));
    mUi->label_Bobina4->setText(QString::number(bobinaId++));
    mUi->label_Bobina5->setText(QString::number(bobinaId++));
    mUi->label_Bobina6->setText(QString::number(bobinaId++));
    mUi->label_Bobina7->setText(QString::number(bobinaId++));
    mUi->label_Bobina8->setText(QString::number(bobinaId++));
    mUi->label_Bobina9->setText(QString::number(bobinaId++));
    mUi->label_Bobina10->setText(QString::number(bobinaId++));
    mUi->label_Bobina11->setText(QString::number(bobinaId++));
    mUi->label_Bobina12->setText(QString::number(bobinaId++));
    mUi->label_Bobina13->setText(QString::number(bobinaId++));
    mUi->label_Bobina14->setText(QString::number(bobinaId++));
    mUi->label_Bobina15->setText(QString::number(bobinaId++));
    mUi->label_Bobina16->setText(QString::number(bobinaId++));
    mUi->label_Bobina17->setText(QString::number(bobinaId++));
    mUi->label_Bobina18->setText(QString::number(bobinaId++));
    mUi->label_Bobina19->setText(QString::number(bobinaId++));
    mUi->label_Bobina20->setText(QString::number(bobinaId++));
}

Medida
Extrusora::getMedida() const
{
    QStringList currentText = mUi->comboBox->currentText().split("/");
    return Medida(currentText.first(), currentText.last());
}

std::vector<IdAndKilos>
Extrusora::getBobinasData() const
{
    std::vector<IdAndKilos> returnData;

    if (mUi->doubleSpinBox_1->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina1->text(), QString::number(mUi->doubleSpinBox_1->value())));
    if (mUi->doubleSpinBox_2->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina2->text(), QString::number(mUi->doubleSpinBox_2->value())));
    if (mUi->doubleSpinBox_3->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina3->text(), QString::number(mUi->doubleSpinBox_3->value())));
    if (mUi->doubleSpinBox_4->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina4->text(), QString::number(mUi->doubleSpinBox_4->value())));
    if (mUi->doubleSpinBox_5->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina5->text(), QString::number(mUi->doubleSpinBox_5->value())));
    if (mUi->doubleSpinBox_6->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina6->text(), QString::number(mUi->doubleSpinBox_6->value())));
    if (mUi->doubleSpinBox_7->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina7->text(), QString::number(mUi->doubleSpinBox_7->value())));
    if (mUi->doubleSpinBox_8->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina8->text(), QString::number(mUi->doubleSpinBox_8->value())));
    if (mUi->doubleSpinBox_9->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina9->text(), QString::number(mUi->doubleSpinBox_9->value())));
    if (mUi->doubleSpinBox_10->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina10->text(), QString::number(mUi->doubleSpinBox_10->value())));
    if (mUi->doubleSpinBox_11->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina11->text(), QString::number(mUi->doubleSpinBox_11->value())));
    if (mUi->doubleSpinBox_12->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina12->text(), QString::number(mUi->doubleSpinBox_12->value())));
    if (mUi->doubleSpinBox_13->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina13->text(), QString::number(mUi->doubleSpinBox_13->value())));
    if (mUi->doubleSpinBox_14->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina14->text(), QString::number(mUi->doubleSpinBox_14->value())));
    if (mUi->doubleSpinBox_15->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina15->text(), QString::number(mUi->doubleSpinBox_15->value())));
    if (mUi->doubleSpinBox_16->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina16->text(), QString::number(mUi->doubleSpinBox_16->value())));
    if (mUi->doubleSpinBox_17->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina17->text(), QString::number(mUi->doubleSpinBox_17->value())));
    if (mUi->doubleSpinBox_18->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina18->text(), QString::number(mUi->doubleSpinBox_18->value())));
    if (mUi->doubleSpinBox_19->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina19->text(), QString::number(mUi->doubleSpinBox_19->value())));
    if (mUi->doubleSpinBox_20->value() != 0) returnData.emplace_back(IdAndKilos(mUi->label_Bobina20->text(), QString::number(mUi->doubleSpinBox_20->value())));

    return returnData;
}
