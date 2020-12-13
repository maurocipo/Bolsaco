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

public slots:
    void on_timerTimeout();
    
private slots:
    void on_operarioLogin();
    void on_administratorLogin();

    void on_showError(const QString& aError);
    void on_showWarning(const QString& aWarning);
    void on_showInfo(const QString& aInfo);
    void on_clearStatusBar();

    void on_statusBarFadeOutFinished();
    void on_statusBarFadeInFinished();

    void on_pushButton_CerrarSesion_clicked();

protected:
    //void resizeEvent(QResizeEvent *event);

private:

    void hideViews();
    void setupDataBase();
    bool setUser();


    // Views
    Admin* mAdmin;
    CargaTareas* mCargaTareas;
    InicioSesion* mInicioSesion;

    unsigned int mFadeCycles;
    QPropertyAnimation* mStatusBarFadeInAnimation;
    QPropertyAnimation* mStatusBarFadeOutAnimation;

    QTimer* mTimer;

    NotificationSender* mNotificationSender;
    QSqlDatabase mDb;
    Ui::MainWindow* mUi;

};
#endif // MAINWINDOW_H
