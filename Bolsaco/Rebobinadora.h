#ifndef REBOBINADORA_H
#define REBOBINADORA_H

#include <QWidget>

namespace Ui {
class Rebobinadora;
}

class Rebobinadora : public QWidget
{
    Q_OBJECT

public:
    explicit Rebobinadora(QWidget *parent = nullptr);
    ~Rebobinadora();

private:
    Ui::Rebobinadora *ui;
};

#endif // REBOBINADORA_H
