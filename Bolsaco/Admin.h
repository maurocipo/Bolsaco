#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>

namespace Ui {
class Admin;
}

class ActividadPorEmpleadoOptions;
class DatosBobinasOptions;
class DatosEmpleadosOptions;
class QPrintPreviewDialog;
class QPrinter;
class QTextDocument;
class NotificationSender;
class ProduccionPorMaquinaOptions;
class TrazabilidadPorBobinaOptions;

class Admin : public QWidget
{
    Q_OBJECT

public:
    explicit Admin(NotificationSender* aNotificationSender, QWidget *parent = nullptr);
    ~Admin();

    void hideOptions();

private slots:

    void on_pushButton_ActPorEmpleado_pressed();

    void on_pushButton_GenerarReporte_pressed();

    void on_pushButton_TrazBobinas_pressed();

    void printPreview(QPrinter*);

    void on_pushButton_ProdPorMaquina_pressed();

    void on_pushButton_AgregarEmpleado_pressed();

    void on_pushButton_pressed();

    void on_pushButton_2_pressed();

private:
    ActividadPorEmpleadoOptions* mActividadPorEmpleadoOptions;
    DatosBobinasOptions* mDatosBobinasOptions;
    DatosEmpleadosOptions* mDatosEmpleadosOptions;
    NotificationSender* mNotificationSender;
    ProduccionPorMaquinaOptions* mProduccionPorMaquinaOptions;
    TrazabilidadPorBobinaOptions* mTrazabilidadPorBobinaOptions;

    QPrintPreviewDialog* mQPrintPreviewDialog;
    QPrinter* mQPrinter;
    QTextDocument* mQTextBrowser;

    Ui::Admin *mUi;
};

#endif // ADMIN_H
