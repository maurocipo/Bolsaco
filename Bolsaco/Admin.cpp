#include "Admin.h"
#include "ui_Admin.h"

#include <QScreen>
#include <QPrintPreviewWidget>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QTextDocument>
#include <QToolBar>

#include "ActividadPorEmpleadoOptions.h"
#include "DataBaseUtils.h"
#include "NotificationSender.h"
#include "TrazabilidadPorBobinaOptions.h"

using namespace DataBaseUtils;

Admin::Admin(NotificationSender* aNotificationSender, QWidget *parent) :
    QWidget(parent),
    mNotificationSender(aNotificationSender),
    mUi(new Ui::Admin)
{
    mUi->setupUi(this);

    mActividadPorEmpleadoOptions = new ActividadPorEmpleadoOptions(mNotificationSender, mUi->widgetAvailableOptions);
    mTrazabilidadPorBobinaOptions = new TrazabilidadPorBobinaOptions(mUi->widgetAvailableOptions);

    mQTextBrowser = new QTextDocument();
    mQPrinter = new QPrinter(QPrinter::ScreenResolution);
    mQPrinter->setFullPage(true);
    mQPrinter->setPaperSize(QPrinter::A4);
    mQPrinter->setOrientation(QPrinter::Portrait);

    mQPrintPreviewDialog = new QPrintPreviewDialog(mQPrinter, this);
    mQPrintPreviewDialog->setWindowFlag(Qt::Window);
    mQPrintPreviewDialog->setWindowTitle("Vista Previa de Impresión");
    QList<QPrintPreviewWidget*> list = mQPrintPreviewDialog->findChildren<QPrintPreviewWidget*>();
    if(!list.isEmpty()) {
      list.first()->setZoomMode(QPrintPreviewWidget::FitToWidth);
    }

    connect(mQPrintPreviewDialog, SIGNAL(paintRequested(QPrinter *)), SLOT(printPreview(QPrinter *)));

    hideOptions();
}

Admin::~Admin()
{
    delete mUi;
}

void
Admin::hideOptions()
{
    mActividadPorEmpleadoOptions->hide();
    mTrazabilidadPorBobinaOptions->hide();

    mUi->pushButton_GenerarReporte->hide();
}

void
Admin::on_pushButton_ActPorEmpleado_pressed()
{
    hideOptions();

    mActividadPorEmpleadoOptions->show();
    mActividadPorEmpleadoOptions->setEnabled(true);
    mUi->pushButton_GenerarReporte->show();
}

void
Admin::on_pushButton_TrazBobinas_pressed()
{
    hideOptions();

    mTrazabilidadPorBobinaOptions->show();
    mUi->pushButton_GenerarReporte->show();
}

void
Admin::on_pushButton_GenerarReporte_pressed()
{
    QString htmlTable;
    if (mActividadPorEmpleadoOptions->isVisible()){
        Result<QString> res = mActividadPorEmpleadoOptions->getHTMLTable();
        if (res.status() == Status::FAILED) {
            mNotificationSender->emitShowError(res.error());
            return;
        }
        htmlTable = res.value();
    } else if (mTrazabilidadPorBobinaOptions->isVisible()) {
        Result<QString> res = mTrazabilidadPorBobinaOptions->getHTMLTable();
        if (res.status() == Status::FAILED) {
            mNotificationSender->emitShowError(res.error());
            return;
        }
        htmlTable = res.value();
    } else if (/*mProduccionPorMaquinaOptions->isVisible()*/false) {

    }
    QString fullHtmlCode = "<html><meta charset='utf-8'><body>";

    QString header;
    header.append("<h1 style='color: #5e9ca0; text-align: center;'>BOLSACO S.A.</h1>");
    header.append("<p><strong>Direcci&oacute;n:</strong> Sarasa XXXX B&ordm; Sarlanga, C&oacute;rdoba Capital</p>");
    header.append("<p style='text-align: justify;'><strong>Tel.</strong>: 0351-4123156</p>");
    header.append("<p style='text-align: justify;'><strong>Cel.</strong>: 0351-150123456</p>");
    header.append("<hr />");
    fullHtmlCode.append(header);

    fullHtmlCode.append(htmlTable);

    fullHtmlCode.append("</body></html>");

    mQTextBrowser->setHtml(fullHtmlCode);

    const int squareSize = QGuiApplication::primaryScreen()->availableGeometry().height() - 150;
    mQPrintPreviewDialog->resize(squareSize, squareSize);

    QList<QPrintPreviewWidget*> list = mQPrintPreviewDialog->findChildren<QPrintPreviewWidget*>();
    if(!list.isEmpty()) {
      list.first()->hide();
      list.first()->updatePreview();
      list.first()->show();
    }

    mQPrintPreviewDialog->exec();

    mNotificationSender->emitShowWarning(mNotificationSender->WARN_ADMIN_USER_LOGGED);
}

void Admin::printPreview(QPrinter *printer)
{
    mQTextBrowser->print(printer);
}
