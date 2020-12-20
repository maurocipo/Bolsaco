#ifndef DATOSEMPLEADOSOPTIONS_H
#define DATOSEMPLEADOSOPTIONS_H

#include <QItemDelegate>
#include <QItemDelegate>
#include <QLineEdit>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QCheckBox>
#include <QWidget>

class QSqlTableModel;
class NotificationSender;

namespace Ui {
class DatosEmpleadosOptions;
}

class MyDelegate : public QItemDelegate
{
    Q_OBJECT
signals:
    void clearStatusBar() const;
    void errorOnTableDelegate(const QString aError) const;
    void updatePasswordForUser(const QString aDNI) const;
    void storeOldUser(const QString aDNI) const;

public:
    explicit MyDelegate(QObject *parent) : QItemDelegate(parent) {}

    // Create Editor when we construct MyDelegate
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    // Then, we set the Editor
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    // When we modify data, this model reflect the change
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    // Give the SpinBox the info on size and location
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void emitClearStatusBar() const {emit clearStatusBar();}
    void emitErrorOnTableDelegate(const QString& aError) const {emit errorOnTableDelegate(aError);}
    void emitUpdatePasswordForUser(const QString& aDNI) const {emit updatePasswordForUser(aDNI);}
    void emitStoreOldUser(const QString& aDNI) const {emit storeOldUser(aDNI);}

    QString mOldDNI;

public slots:

private:


};

class DatosEmpleadosOptions : public QWidget
{
    Q_OBJECT
signals:
    void passwordNeedsUpdate(const QString& aOldDNI, const QString& aNewDNI);

public:
    explicit DatosEmpleadosOptions(NotificationSender* aNotificationSender, QWidget *parent = nullptr);
    ~DatosEmpleadosOptions();

    void setAgregarEmpleadoLayout();
    void setModificarEmpleadoLayout();

    void clear();

private slots:
    void on_lineEdit_DNI_textChanged(const QString &arg1);

    void on_toolButton_Nuevo_pressed();

    void on_lineEdit_NombreYApellido_textChanged(const QString &arg1);

    void on_errorOnTableDelegate(const QString& aError) const;
    void on_updatePasswordForUser(const QString& aDNI);

    void on_emitClearStatusBar() const;

    void on_storeOldUser(const QString& aDNI);

private:
    void emitPasswordNeedsUpdate(const QString& aOldDNI, const QString& aNewDNI);

    QString mOldDNI;

    QSqlTableModel* mSqlTableModel;
    MyDelegate* mDelegate;

    NotificationSender* mNotificationSender;
    Ui::DatosEmpleadosOptions *mUi;
};

#endif // DATOSEMPLEADOSOPTIONS_H
