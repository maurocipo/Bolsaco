#ifndef ACTIVIDADPOREMPLEADOOPTIONS_H
#define ACTIVIDADPOREMPLEADOOPTIONS_H

#include <QWidget>

namespace Ui {
class ActividadPorEmpleadoOptions;
}

class ActividadPorEmpleadoOptions : public QWidget
{
    Q_OBJECT

public:
    explicit ActividadPorEmpleadoOptions(QWidget *parent = nullptr);
    ~ActividadPorEmpleadoOptions();

private slots:
    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::ActividadPorEmpleadoOptions *mUi;

    QString mSelectedOperario;
};

#endif // ACTIVIDADPOREMPLEADOOPTIONS_H
