#ifndef DATOSBOBINASOPTIONS_H
#define DATOSBOBINASOPTIONS_H

#include <QWidget>

namespace Ui {
class DatosBobinasOptions;
}

class NotificationSender;

class DatosBobinasOptions : public QWidget
{
    Q_OBJECT

public:
    explicit DatosBobinasOptions(NotificationSender* aNotificationSender, QWidget *parent = nullptr);
    ~DatosBobinasOptions();

    void clear();

private slots:
    void on_toolButton_Ancho_pressed();

    void on_toolButton_Micronaje_pressed();

    void on_toolButton_Largo_pressed();

private:
    NotificationSender* mNotificationSender;
    Ui::DatosBobinasOptions *mUi;
};

#endif // DATOSBOBINASOPTIONS_H
