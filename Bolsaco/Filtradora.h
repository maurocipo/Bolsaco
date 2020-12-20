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

    void clear();

    double getKilos() const;

    bool hasUnsavedWork() const;

private:
    Ui::Filtradora *mUi;
};

#endif // FILTRADORA_H
