#ifndef PRODUCCIONPORMAQUINAOPTIONS_H
#define PRODUCCIONPORMAQUINAOPTIONS_H

#include <QWidget>

namespace Ui {
class ProduccionPorMaquinaOptions;
}

class ProduccionPorMaquinaOptions : public QWidget
{
    Q_OBJECT

public:
    explicit ProduccionPorMaquinaOptions(QWidget *parent = nullptr);
    ~ProduccionPorMaquinaOptions();

private:
    Ui::ProduccionPorMaquinaOptions *ui;
};

#endif // PRODUCCIONPORMAQUINAOPTIONS_H
