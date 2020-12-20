#ifndef ACTIVIDADPOREMPLEADOOPTIONS_H
#define ACTIVIDADPOREMPLEADOOPTIONS_H

#include <QWidget>

#include "Result.h"

namespace Ui {
class ActividadPorEmpleadoOptions;
}

class NotificationSender;

class ActividadPorEmpleadoOptions : public QWidget
{
    Q_OBJECT

public:
    explicit ActividadPorEmpleadoOptions(NotificationSender* aNotificationSender, QWidget *parent = nullptr);
    ~ActividadPorEmpleadoOptions();

    Result<QString> getHTMLTable();

    void clear();

private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_checkBox_FechaDesde_stateChanged(int arg1);

    void on_checkBox_FechaHasta_stateChanged(int arg1);

    void on_checkBox_Tarea_stateChanged(int arg1);

    void on_checkBox_Maquina_stateChanged(int arg1);

    void on_lineEdit_DNI_textChanged(const QString &arg1);

    void on_lineEdit_Nombre_textChanged(const QString &arg1);

    void on_comboBox_Tarea_currentIndexChanged(int index);

private:
    Result<QString> buildTareaCortado(const QString& aUserId, const QString& aDesde, const QString& aHasta) const;
    Result<QString> buildTareaExtrusado(const QString& aUserId, const QString& aDesde, const QString& aHasta) const;
    Result<QString> buildTareaFiltrado(const QString& aUserId, const QString& aDesde, const QString& aHasta) const;
    Result<QString> buildTareaLavado(const QString& aUserId, const QString& aDesde, const QString& aHasta) const;
    Result<QString> buildTareaRebobinado(const QString& aUserId, const QString& aDesde, const QString& aHasta) const;

    void fillComboboxMaquina();
    void fillComboboxTarea();

    Ui::ActividadPorEmpleadoOptions *mUi;

    NotificationSender* mNotificationSender;
    QString mSelectedOperario;
};

#endif // ACTIVIDADPOREMPLEADOOPTIONS_H
