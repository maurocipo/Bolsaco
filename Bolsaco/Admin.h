#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>

namespace Ui {
class Admin;
}

class ActividadPorEmpleadoOptions;
class QPrintPreviewDialog;
class QPrinter;
class QTextDocument;
class NotificationSender;
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

    void printPreview(QPrinter*);

private:
    void hideOptions();

    ActividadPorEmpleadoOptions* mActividadPorEmpleadoOptions;
    NotificationSender* mNotificationSender;
    TrazabilidadPorBobinaOptions* mTrazabilidadPorBobinaOptions;

    QPrintPreviewDialog* mQPrintPreviewDialog;
    QPrinter* mQPrinter;
    QTextDocument* mQTextBrowser;

    Ui::Admin *mUi;
};

#endif // ADMIN_H
