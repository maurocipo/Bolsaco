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

signals:
    void showError(const QString& aError);
    void showWarning(const QString& aWarning);
    void showInfo(const QString& aInfo);
    void clearStatusBar();
    void newUserAdded(const QString& aDNI);
    void passwordNeedsUpdate(const QString& aOldDNI, const QString& aNewDNI);

public:
    explicit NotificationSender(QWidget *aParent = nullptr);
    ~NotificationSender();

    static const char* WARN_ADMIN_USER_LOGGED;

    void emitShowError(const QString& aError);
    void emitShowWarning(const QString& aWarning);
    void emitShowInfo(const QString& aInf);
    void emitClearStatusBar();
    void emitNewUserAdded(const QString& aDNI);
    void emitPasswordNeedsUpdate(const QString& aOldDNI, const QString& aNewDNI);

};

#endif // NOTIFICATIONSENDER_H
