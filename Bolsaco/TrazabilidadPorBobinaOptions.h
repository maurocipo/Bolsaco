#ifndef TRAZABILIDADPORBOBINAOPTIONS_H
#define TRAZABILIDADPORBOBINAOPTIONS_H

#include <QWidget>

namespace Ui {
class TrazabilidadPorBobinaOptions;
}

class TrazabilidadPorBobinaOptions : public QWidget
{
    Q_OBJECT

public:
    explicit TrazabilidadPorBobinaOptions(QWidget *parent = nullptr);
    ~TrazabilidadPorBobinaOptions();

private:
    Ui::TrazabilidadPorBobinaOptions *ui;
};

#endif // TRAZABILIDADPORBOBINAOPTIONS_H
