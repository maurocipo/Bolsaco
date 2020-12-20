#ifndef REBOBINADORA_H
#define REBOBINADORA_H

#include <QWidget>

namespace Ui {
class Rebobinadora;
}

struct ProductIdAndKilos {
    ProductIdAndKilos(const int aProductId, const double aKilos)
        : mProductId(aProductId),
          mKilos(aKilos) {}

    int mProductId;
    double mKilos;
};

class Rebobinadora : public QWidget
{
    Q_OBJECT

public:

    explicit Rebobinadora(QWidget *parent = nullptr);
    ~Rebobinadora();

    void clear();
    std::vector<ProductIdAndKilos> getKilos() const;

    bool hasUnsavedWork() const;

private:
    Ui::Rebobinadora *mUi;
};

#endif // REBOBINADORA_H
