#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>

namespace Ui {
class Admin;
}

class ActividadPorEmpleadoOptions;
class NotificationSender;
class PrintPreview;
class TrazabilidadPorBobinaOptions;

class Admin : public QWidget
{
    Q_OBJECT

public:
    explicit Admin(NotificationSender* aNotificationSender, QWidget *parent = nullptr);
    ~Admin();

private slots:

    void on_pushButton_ActPorEmpleado_pressed();

    void on_pushButton_GenerarReporte_pressed();

    void on_pushButton_TrazBobinas_pressed();

private:
    void hideOptions();

    ActividadPorEmpleadoOptions* mActividadPorEmpleadoOptions;
    NotificationSender* mNotificationSender;
    PrintPreview* mPrintPreview;
    TrazabilidadPorBobinaOptions* mTrazabilidadPorBobinaOptions;

    Ui::Admin *mUi;
};

#endif // ADMIN_H
