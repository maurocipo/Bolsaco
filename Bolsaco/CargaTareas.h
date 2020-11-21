#ifndef CARGATAREAS_H
#define CARGATAREAS_H

#include <QWidget>

namespace Ui {
class CargaTareas;
}

class Cortadora;
class Extrusora;
class Filtradora;
class Lavadora;
class Rebobinadora;

class CargaTareas : public QWidget
{
    Q_OBJECT

public:
    explicit CargaTareas(QWidget *parent = nullptr);
    ~CargaTareas();

    void setCurrentUser(const unsigned int aUser) {mCurrentUser = aUser;}

private slots:
    void on_comboBox_numero_currentIndexChanged(int index);
    void on_comboBox_tipoDeMaquina_currentIndexChanged(int index);

    void on_pushButton_guardar_pressed();

private:
    void hideMaquinas() const;

    // Maquinas
    Cortadora* mCortadora;
    Extrusora* mExtrusora;
    Filtradora* mFiltradora;
    Lavadora* mLavadora;
    Rebobinadora* mRebobinadora;

    unsigned int mCurrentUser;

    Ui::CargaTareas *mUi;
};

#endif // CARGATAREAS_H
