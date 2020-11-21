#ifndef NOTIFICATIONSENDER_H
#define NOTIFICATIONSENDER_H

#include <iostream>
#include <QString>
#include <QWidget>

// Macro used for dev purposes
#define ERROR(msg) { std::cout << "ERROR in file" << __FILE__ << " ln " << __LINE__ << ": " << msg << std::endl;}

class NotificationSender : public QWidget
{
    Q_OBJECT

/*signals:
    void ordenDeTrabajoStored(const QString& aID);
    void showError(const QString& aError);
    void showWarning(const QString& aWarning);
    void showInfo(const QString& aInfo);
    void clearStatusBar();
*/
public:
    explicit NotificationSender(QWidget *aParent = nullptr);
    ~NotificationSender();

/*    void emitOrdenDeTrabajoStored(const QString& aID);
    void emitShowError(const QString& aError);
    void emitShowWarning(const QString& aWarning);
    void emitShowInfo(const QString& aInf);
    void emitClearStatusBar();
*/
};

#endif // NOTIFICATIONSENDER_H
