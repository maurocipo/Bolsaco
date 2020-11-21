#ifndef FILTRADORA_H
#define FILTRADORA_H

#include <QWidget>

namespace Ui {
class Filtradora;
}

class Filtradora : public QWidget
{
    Q_OBJECT

public:
    explicit Filtradora(QWidget *parent = nullptr);
    ~Filtradora();

private:
    Ui::Filtradora *ui;
};

#endif // FILTRADORA_H
