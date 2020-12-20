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
#include "DatosBobinasOptions.h"
#include "DatosEmpleadosOptions.h"
#include "NotificationSender.h"
#include "ProduccionPorMaquinaOptions.h"
#include "TrazabilidadPorBobinaOptions.h"

using namespace DataBaseUtils;

Admin::Admin(NotificationSender* aNotificationSender, QWidget *parent) :
    QWidget(parent),
    mNotificationSender(aNotificationSender),
    mUi(new Ui::Admin)
{
    mUi->setupUi(this);

    mActividadPorEmpleadoOptions = new ActividadPorEmpleadoOptions(mNotificationSender, mUi->widgetAvailableOptions);
    mUi->widgetAvailableOptions->layout()->addWidget(mActividadPorEmpleadoOptions);
    mDatosBobinasOptions = new DatosBobinasOptions(mNotificationSender, mUi->widgetAvailableOptions);
    mUi->widgetAvailableOptions->layout()->addWidget(mDatosBobinasOptions);
    mDatosEmpleadosOptions = new DatosEmpleadosOptions(mNotificationSender, mUi->widgetAvailableOptions);
    mUi->widgetAvailableOptions->layout()->addWidget(mDatosEmpleadosOptions);
    mTrazabilidadPorBobinaOptions = new TrazabilidadPorBobinaOptions(mUi->widgetAvailableOptions);
    mUi->widgetAvailableOptions->layout()->addWidget(mTrazabilidadPorBobinaOptions);
    mProduccionPorMaquinaOptions = new ProduccionPorMaquinaOptions(mUi->widgetAvailableOptions);
    mUi->widgetAvailableOptions->layout()->addWidget(mProduccionPorMaquinaOptions);

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
    mActividadPorEmpleadoOptions->clear();
    mActividadPorEmpleadoOptions->hide();

    mDatosBobinasOptions->clear();
    mDatosBobinasOptions->hide();

    mDatosEmpleadosOptions->clear();
    mDatosEmpleadosOptions->hide();

    mProduccionPorMaquinaOptions->clear();
    mProduccionPorMaquinaOptions->hide();

    mTrazabilidadPorBobinaOptions->clear();
    mTrazabilidadPorBobinaOptions->hide();

    mUi->pushButton_GenerarReporte->hide();
}

void
Admin::on_pushButton_ActPorEmpleado_pressed()
{
    hideOptions();

    mActividadPorEmpleadoOptions->show();
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
Admin::on_pushButton_ProdPorMaquina_pressed()
{
    hideOptions();

    mProduccionPorMaquinaOptions->show();
    mUi->pushButton_GenerarReporte->show();
}

void Admin::on_pushButton_AgregarEmpleado_pressed()
{
    hideOptions();

    mDatosEmpleadosOptions->setAgregarEmpleadoLayout();
    mDatosEmpleadosOptions->show();
}

void Admin::on_pushButton_pressed()
{
    hideOptions();

    mDatosEmpleadosOptions->setModificarEmpleadoLayout();
    mDatosEmpleadosOptions->show();
}

void Admin::on_pushButton_2_pressed()
{
    hideOptions();

    mDatosBobinasOptions->show();
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
    } else if (mProduccionPorMaquinaOptions->isVisible()) {
        Result<QString> res = mProduccionPorMaquinaOptions->getHTMLTable();
        if (res.status() == Status::FAILED) {
            mNotificationSender->emitShowError(res.error());
            return;
        }
        htmlTable = res.value();
    }
    QString fullHtmlCode = "<html><meta charset='utf-8'><body>";

    QString header;
    header.append("<h1 style='color: #5e9ca0; text-align: center;'>BOLSACO S.A.</h1>");
    header.append("<p><strong>Direcci&oacute;n:</strong> Mar&iacute;a Lastenia 550 - C&oacute;rdoba</p>");
    header.append("<p style='text-align: justify;'><strong>Tel.</strong>: 0351-4994893</p>");
    header.append("<p style='text-align: justify;'><strong>Mail de contacto: </strong>: bolsaco@bolsaco.com.ar</p>");
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
