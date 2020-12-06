#include "Admin.h"
#include "ui_Admin.h"

#include "ActividadPorEmpleadoOptions.h"
#include "DataBaseUtils.h"
#include "NotificationSender.h"
#include "PrintPreview.h"
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

    mPrintPreview = new PrintPreview();

    hideOptions();
}

Admin::~Admin()
{
    delete mUi;
}

void
Admin::hideOptions()
{
    mPrintPreview->hide();

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

    mPrintPreview->setText(fullHtmlCode);
    mPrintPreview->show();
    mNotificationSender->emitShowWarning(mNotificationSender->WARN_ADMIN_USER_LOGGED);
}
