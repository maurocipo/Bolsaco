#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CargaTareas;
class InicioSesion;
class SimpleCrypt;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *aParent = nullptr);
    ~MainWindow();

private slots:
    void on_successfulLogin();
/*  void on_ordenDeTrabajoStored(QString aID);
    void on_showError(QString aError);
    void on_showWarning(QString aWarning);
    void on_showInfo(QString aInfo);
    void on_clearStatusBar();

    void on_pushButtonHome_clicked();

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

private:
    void setupDataBase();
//    void setupHome();

    // Views
    CargaTareas* mCargaTareas;
    InicioSesion* mInicioSesion;

    SimpleCrypt* mCrypt;
    QSqlDatabase mDb;
    Ui::MainWindow* mUi;

};
#endif // MAINWINDOW_H
