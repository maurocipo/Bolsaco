#ifndef LAVADORA_H
#define LAVADORA_H

#include <QWidget>

namespace Ui {
class Lavadora;
}

class Lavadora : public QWidget
{
    Q_OBJECT

public:
    explicit Lavadora(QWidget *parent = nullptr);
    ~Lavadora();

private:
    Ui::Lavadora *ui;
};

#endif // LAVADORA_H
