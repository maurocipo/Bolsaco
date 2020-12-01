#ifndef ESTRUSORA_H
#define ESTRUSORA_H

#include <QWidget>

namespace Ui {
class Extrusora;
}

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
    explicit Extrusora(QWidget *parent = nullptr);
    ~Extrusora();

    void clear();
    void fillMedidas(const QStringList& aMedidas);
    void setBobinasIds(const QString& aCurrentGratestBobinaId);

    Medida getMedida() const;
    std::vector<IdAndKilos> getBobinasData() const;

private:
    Ui::Extrusora *mUi;
};

#endif // ESTRUSORA_H
