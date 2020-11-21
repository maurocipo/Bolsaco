#ifndef ESTRUSORA_H
#define ESTRUSORA_H

#include <QWidget>

namespace Ui {
class Extrusora;
}

class Extrusora : public QWidget
{
    Q_OBJECT

public:
    explicit Extrusora(QWidget *parent = nullptr);
    ~Extrusora();

private:
    Ui::Extrusora *mUi;
};

#endif // ESTRUSORA_H
