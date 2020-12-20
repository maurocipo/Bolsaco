#ifndef PRODUCCIONPORMAQUINAOPTIONS_H
#define PRODUCCIONPORMAQUINAOPTIONS_H

#include <QWidget>

#include "Result.h"

namespace Ui {
class ProduccionPorMaquinaOptions;
}

class ProduccionPorMaquinaOptions : public QWidget
{
    Q_OBJECT

public:
    explicit ProduccionPorMaquinaOptions(QWidget *parent = nullptr);
    ~ProduccionPorMaquinaOptions();

    Result<QString> getHTMLTable();

    void clear();
private:
    void fillComboboxMaquina();

    Result<QString>
    getCortadoraTable(const bool aTodasLasMaquinas, const QString& aNumeroMaquina, const QString& aDesde, const QString& aHasta) const;
    Result<QString>
    getExtrusoraTable(const bool aTodasLasMaquinas, const QString& aNumeroMaquina, const QString& aDesde, const QString& aHasta) const;
    Result<QString>
    getFiltradoraTable(const bool aTodasLasMaquinas, const QString& aNumeroMaquina, const QString& aDesde, const QString& aHasta) const;
    Result<QString>
    getLavadoraTable(const bool aTodasLasMaquinas, const QString& aNumeroMaquina, const QString& aDesde, const QString& aHasta) const;
    Result<QString>
    getRebobinadoraTable(const bool aTodasLasMaquinas, const QString& aNumeroMaquina, const QString& aDesde, const QString& aHasta) const;

    Ui::ProduccionPorMaquinaOptions *mUi;
};

#endif // PRODUCCIONPORMAQUINAOPTIONS_H
