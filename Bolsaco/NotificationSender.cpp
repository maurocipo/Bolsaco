#include "NotificationSender.h"

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
