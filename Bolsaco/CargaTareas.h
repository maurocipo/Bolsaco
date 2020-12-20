#ifndef CARGATAREAS_H
#define CARGATAREAS_H

#include <QWidget>

#include "Result.h"

namespace Ui {
class CargaTareas;
}

class Cortadora;
class Extrusora;
class Filtradora;
class Lavadora;
class NotificationSender;
class Rebobinadora;

class CargaTareas : public QWidget
{
    Q_OBJECT

public:
    explicit CargaTareas(NotificationSender* aNotificationSender, QWidget *parent = nullptr);
    ~CargaTareas();

    void setCurrentUser(const unsigned int aUser) {mCurrentUser = aUser;}
    void hideMaquinas() const;

    void clearOptions();

    bool hasUnsavedWork() const;

private slots:
    void on_comboBox_numero_currentIndexChanged(int index);
    void on_comboBox_tipoDeMaquina_currentIndexChanged(int index);
    void on_pushButton_guardar_pressed();

private:
    Result<void> storeTareaCortado(const QString& aIdMaquina, const QString& aIdOperario);
    Result<void> storeTareaExtrusado(const QString& aIdMaquina, const QString& aIdOperario);
    Result<void> storeTareaFiltrado(const QString& aIdMaquina, const QString& aIdOperario);
    Result<void> storeTareaLavado(const QString& aIdMaquina, const QString& aIdOperario);
    Result<void> storeTareaRebobinado(const QString& aIdMaquina, const QString& aIdOperario);

    QStringList buildLargosCortesForDisplay() const;

    // Maquinas
    Cortadora* mCortadora;
    Extrusora* mExtrusora;
    Filtradora* mFiltradora;
    Lavadora* mLavadora;
    Rebobinadora* mRebobinadora;

    unsigned int mCurrentUser;

    NotificationSender* mNotificationSender;
    Ui::CargaTareas *mUi;
};

#endif // CARGATAREAS_H
