#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QStatusBar>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Admin;
class CargaTareas;
class QPropertyAnimation;
class InicioSesion;
class NotificationSender;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *aParent = nullptr);
    ~MainWindow();

private slots:
    void on_operarioLogin();
    void on_administratorLogin();

    void on_showError(const QString& aError);
    void on_showWarning(const QString& aWarning);
    void on_showInfo(const QString& aInfo);
    void on_clearStatusBar();

    void on_statusBarFadeOutFinished();
    void on_statusBarFadeInFinished();

/*    void on_pushButtonHome_clicked();

    void on_pushButtonHome_pressed();

    void on_pushButtonHome_released();

    void on_pushButtonHome_Ingreso_clicked();

    void on_pushButtonHome_Ingreso_pressed();

    void on_pushButtonHome_Ingreso_released();

    void on_pushButtonHome_Entrega_pressed();

    void on_pushButtonHome_Entrega_released();

    void on_pushButtonHome_LibroPoli_pressed();

    void on_pushButtonHome_LibroPoli_released();

    void on_pushButtonHome_Inventario_pressed();

    void on_pushButtonHome_Inventario_released();

    void on_pushButtonHome_Entrega_clicked();

    void on_pushButtonHome_LibroPoli_clicked();

    void on_pushButtonHome_Inventario_clicked();
*/

    void on_pushButton_CerrarSesion_clicked();

private:

    void hideViews();
    void setupDataBase();
    bool setUser();
//    void setupHome();

    // Views
    Admin* mAdmin;
    CargaTareas* mCargaTareas;
    InicioSesion* mInicioSesion;

    unsigned int mFadeCycles;
    QPropertyAnimation* mStatusBarFadeInAnimation;
    QPropertyAnimation* mStatusBarFadeOutAnimation;

    NotificationSender* mNotificationSender;
    QSqlDatabase mDb;
    Ui::MainWindow* mUi;

};
#endif // MAINWINDOW_H
