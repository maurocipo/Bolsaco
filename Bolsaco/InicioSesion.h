#ifndef INICIOSESION_H
#define INICIOSESION_H

#include <QWidget>

namespace Ui {
class InicioSesion;
}

class SimpleCrypt;

class InicioSesion : public QWidget
{
    Q_OBJECT
signals:
    void successfulLogin();

public:
    explicit InicioSesion(SimpleCrypt& aCrypt,
                          QWidget* parent = nullptr);
    ~InicioSesion();

    unsigned int getCurrentUser() const {return mCurrentUser;}

    void emitSuccessfulLogin();

private slots:
    void on_pushButton_IniciarSesion_pressed();

    void on_lineEdit_Contrasena_returnPressed();

private:
    unsigned int mCurrentUser;
    SimpleCrypt* mCrypt;
    Ui::InicioSesion *mUi;
};

#endif // INICIOSESION_H
