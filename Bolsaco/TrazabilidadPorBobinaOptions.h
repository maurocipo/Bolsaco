#ifndef TRAZABILIDADPORBOBINAOPTIONS_H
#define TRAZABILIDADPORBOBINAOPTIONS_H

#include <QWidget>

#include "DataBaseUtils.h"
#include "Result.h"

namespace Ui {
class TrazabilidadPorBobinaOptions;
}

class TrazabilidadPorBobinaOptions : public QWidget
{
    Q_OBJECT

public:
    explicit TrazabilidadPorBobinaOptions(QWidget *parent = nullptr);
    ~TrazabilidadPorBobinaOptions();

    Result<QString> getHTMLTable() const;

private:
    std::vector<DataBaseUtils::KeyAndValue> getBobinasConditions() const;

    Ui::TrazabilidadPorBobinaOptions *mUi;
};

#endif // TRAZABILIDADPORBOBINAOPTIONS_H
