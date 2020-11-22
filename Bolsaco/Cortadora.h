#ifndef CORTADORA_H
#define CORTADORA_H

#include <QWidget>

#include "DataBaseUtils.h"

namespace Ui {
class Cortadora;
}

class Cortadora : public QWidget
{
    Q_OBJECT

public:
    explicit Cortadora(QWidget *parent = nullptr);
    ~Cortadora();

    std::vector<DataBaseUtils::KeyAndValue> getData() const;

private:
    Ui::Cortadora *ui;
};

#endif // CORTADORA_H
