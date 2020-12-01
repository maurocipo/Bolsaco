#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>

namespace Ui {
class Admin;
}

class ActividadPorEmpleadoOptions;
class PrintPreview;

class Admin : public QWidget
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr);
    ~Admin();

private slots:

    void on_pushButton_ActPorEmpleado_pressed();

    void on_pushButton_Buscar_pressed();

private:
    void hideOptions();

    ActividadPorEmpleadoOptions* mActividadPorEmpleadoOptions;
    PrintPreview* mPrintPreview;

    Ui::Admin *mUi;
};

#endif // ADMIN_H
