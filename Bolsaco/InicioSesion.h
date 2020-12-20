#ifndef INICIOSESION_H
#define INICIOSESION_H

#include <QWidget>

namespace Ui {
class InicioSesion;
}

class NotificationSender;
class SimpleCrypt;

class InicioSesion : public QWidget
{
    Q_OBJECT
signals:
    void operarioLogin();
    void administratorLogin();

public:
    explicit InicioSesion(NotificationSender* aNotificationSender, QWidget* parent = nullptr);
    ~InicioSesion();

    unsigned int getCurrentUser() const {return mCurrentUser;}

    void emitOperarioLogin();
    void emitAdministratorLogin();

private slots:
    void on_pushButton_IniciarSesion_pressed();

    void on_lineEdit_Contrasena_returnPressed();

    void on_lineEdit_DNI_editingFinished();

    void on_newUserAdded(const QString&);

    void on_passwordNeedsUpdate(const QString& aOldDNI, const QString& aNewDNI);

private:
    QString mAccessFile;
    unsigned int mCurrentUser;
    NotificationSender* mNotificationSender;
    SimpleCrypt* mCrypt;
    Ui::InicioSesion *mUi;
};

#endif // INICIOSESION_H
