#ifndef PRODUCCIONPORMAQUINAOPTIONS_H
#define PRODUCCIONPORMAQUINAOPTIONS_H

#include <QWidget>

#include "Result.h"

namespace Ui {
class ProduccionPorMaquinaOptions;
}

class ProduccionPorMaquinaOptions : public QWidget
{
    Q_OBJECT

public:
    explicit ProduccionPorMaquinaOptions(QWidget *parent = nullptr);
    ~ProduccionPorMaquinaOptions();

    Result<QString>
    getHTMLTable();

private:
    void fillComboboxMaquina();

    Ui::ProduccionPorMaquinaOptions *mUi;
};

#endif // PRODUCCIONPORMAQUINAOPTIONS_H
