#ifndef ESTRUSORA_H
#define ESTRUSORA_H

#include <QWidget>

#include "Result.h"

namespace Ui {
class Extrusora;
}

class NotificationSender;

struct Medida {
    Medida(const QString aAncho, const QString aMicronaje)
        : mAncho(aAncho), mMicronaje(aMicronaje) {}

    QString mAncho;
    QString mMicronaje;
};

struct IdAndKilos {
    IdAndKilos(const QString aId, const QString aKilos)
        : mId(aId), mKilos(aKilos) {}

    QString mId;
    QString mKilos;
};

class Extrusora : public QWidget
{
    Q_OBJECT

public:
    explicit Extrusora(NotificationSender* aNotificationSender, QWidget *parent = nullptr);
    ~Extrusora();

    void clear();
    void fillMedidas();
    void setBobinasIds(const QString& aCurrentGratestBobinaId);

    Medida getMedida() const;
    bool hasUnsavedWork() const;
    Result<std::vector<IdAndKilos>> getBobinasData() const;

private:
    NotificationSender* mNotificationSender;
    Ui::Extrusora *mUi;
};

#endif // ESTRUSORA_H
