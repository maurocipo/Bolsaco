#include "Extrusora.h"
#include "ui_Extrusora.h"

#include "DataBaseData.h"
#include "DataBaseUtils.h"
#include "NotificationSender.h"

using namespace DataBaseUtils;

const char* ERROR_ESPACIOS_CONSECUTIVOS = "Error: Llenar espacios seguidos!";

Extrusora::Extrusora(NotificationSender* aNotificationSender, QWidget *parent) :
    QWidget(parent),
    mNotificationSender(aNotificationSender),
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
Extrusora::fillMedidas()
{
    mUi->comboBox->clear();

    QStringList medidas;

    Result<std::vector<KeyAndValue>> result = select(TableNames::MEDIDAS_BOBINAS, "*", {});
    if (result.status() != Status::SUCCEEDED) {
        mNotificationSender->emitShowError(result.error());
        return;
    }
    std::vector<KeyAndValue> data = result.value();


    for (const KeyAndValue& keyAndValue : data) {
        if (keyAndValue.mKey == MedidasBobinasFields::ANCHO) {
            medidas.append(keyAndValue.mValue);
        }
        if (keyAndValue.mKey == MedidasBobinasFields::MICRONAJE) {
            QString& medida = medidas.back();
            medida.append("/" + keyAndValue.mValue);
        }
    }
    mUi->comboBox->addItems(medidas);

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

Result<std::vector<IdAndKilos>>
Extrusora::getBobinasData() const
{
    std::vector<IdAndKilos> returnData;
    bool emptyFieldBeforeThisOne = false;
    if (mUi->doubleSpinBox_1->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina1->text(), QString::number(mUi->doubleSpinBox_1->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_2->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina2->text(), QString::number(mUi->doubleSpinBox_2->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_3->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina3->text(), QString::number(mUi->doubleSpinBox_3->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_4->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina4->text(), QString::number(mUi->doubleSpinBox_4->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_5->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina5->text(), QString::number(mUi->doubleSpinBox_5->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_6->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina6->text(), QString::number(mUi->doubleSpinBox_6->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_7->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina7->text(), QString::number(mUi->doubleSpinBox_7->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_8->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina8->text(), QString::number(mUi->doubleSpinBox_8->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_9->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina9->text(), QString::number(mUi->doubleSpinBox_9->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_10->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina10->text(), QString::number(mUi->doubleSpinBox_10->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_11->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina11->text(), QString::number(mUi->doubleSpinBox_11->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_12->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina12->text(), QString::number(mUi->doubleSpinBox_12->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_13->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina13->text(), QString::number(mUi->doubleSpinBox_13->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_14->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina14->text(), QString::number(mUi->doubleSpinBox_14->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_15->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina15->text(), QString::number(mUi->doubleSpinBox_15->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_16->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina16->text(), QString::number(mUi->doubleSpinBox_16->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_17->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina17->text(), QString::number(mUi->doubleSpinBox_17->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_18->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina18->text(), QString::number(mUi->doubleSpinBox_18->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_19->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina19->text(), QString::number(mUi->doubleSpinBox_19->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    if (mUi->doubleSpinBox_20->value() != 0) {
        if (emptyFieldBeforeThisOne) {
            return Result<std::vector<IdAndKilos>>(Status::FAILED, ERROR_ESPACIOS_CONSECUTIVOS);
        }
        returnData.emplace_back(IdAndKilos(mUi->label_Bobina20->text(), QString::number(mUi->doubleSpinBox_20->value())));
    } else {
        emptyFieldBeforeThisOne = true;
    }

    return returnData;
}

bool
Extrusora::hasUnsavedWork() const
{
    if (mUi->doubleSpinBox_1->value() != 0 || mUi->doubleSpinBox_2->value() != 0 ||
        mUi->doubleSpinBox_3->value() != 0 || mUi->doubleSpinBox_4->value() != 0 ||
        mUi->doubleSpinBox_5->value() != 0 || mUi->doubleSpinBox_6->value() != 0 ||
        mUi->doubleSpinBox_7->value() != 0 || mUi->doubleSpinBox_8->value() != 0 ||
        mUi->doubleSpinBox_9->value() != 0 || mUi->doubleSpinBox_10->value() != 0 ||
        mUi->doubleSpinBox_11->value() != 0 || mUi->doubleSpinBox_12->value() != 0 ||
        mUi->doubleSpinBox_13->value() != 0 || mUi->doubleSpinBox_14->value() != 0 ||
        mUi->doubleSpinBox_15->value() != 0 || mUi->doubleSpinBox_16->value() != 0 ||
        mUi->doubleSpinBox_17->value() != 0 || mUi->doubleSpinBox_18->value() != 0 ||
        mUi->doubleSpinBox_19->value() != 0 || mUi->doubleSpinBox_20->value() != 0) {

        return true;
    } else {
        return false;
    }
}
