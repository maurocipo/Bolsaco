#ifndef CORTADORA_H
#define CORTADORA_H

#include <QWidget>

#include "DataBaseUtils.h"

namespace Ui {
class Cortadora;
}

struct BobinaIdAndCortes {
    BobinaIdAndCortes(const QString& aBobinaId, const QString& aCortes)
        : mBobinaId(aBobinaId), mCortes(aCortes) {}

    QString mBobinaId;
    QString mCortes;
};

class Cortadora : public QWidget
{
    Q_OBJECT

public:
    explicit Cortadora(QWidget *parent = nullptr);
    ~Cortadora();

    void clear();
    void fillLargos(const QStringList& aMedidas);

    QString getLargo() const;
    std::vector<BobinaIdAndCortes> getData() const;

private:
    Ui::Cortadora *mUi;
};

#endif // CORTADORA_H
