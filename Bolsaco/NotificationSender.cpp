#include "NotificationSender.h"

const char* NotificationSender::WARN_ADMIN_USER_LOGGED = "CUIDADO: Login Administrador.";

NotificationSender::NotificationSender(QWidget *aParent)
    : QWidget(aParent)
{
}

NotificationSender::~NotificationSender()
{
}

void
NotificationSender::emitShowError(const QString& aError)
{
    emit showError(aError);
}

void
NotificationSender::emitShowWarning(const QString& aWarning)
{
    emit showWarning(aWarning);
}

void
NotificationSender::emitShowInfo(const QString& aInfo)
{
    emit showInfo(aInfo);
}

void
NotificationSender::emitClearStatusBar()
{
    emit clearStatusBar();
}
