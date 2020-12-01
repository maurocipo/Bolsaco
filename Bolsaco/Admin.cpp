#include "Admin.h"
#include "ui_Admin.h"

#include "ActividadPorEmpleadoOptions.h"
#include "DataBaseUtils.h"
#include "NotificationSender.h"
#include "PrintPreview.h"

using namespace DataBaseUtils;

Admin::Admin(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::Admin)
{
    mUi->setupUi(this);

    mActividadPorEmpleadoOptions = new ActividadPorEmpleadoOptions(mUi->widgetAvailableOptions);

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

    mUi->pushButton_Buscar->hide();
}

void
Admin::on_pushButton_ActPorEmpleado_pressed()
{
    hideOptions();

    mActividadPorEmpleadoOptions->show();
    mUi->pushButton_Buscar->show();
}

void
Admin::on_pushButton_Buscar_pressed()
{
    QString fullHtmlCode = "<html><meta charset='utf-8'><body>";

    QString header;
    header.append("<h1 style='color: #5e9ca0; text-align: center;'>BOLSACO S.A.</h1>");
    header.append("<p><strong>Direcci&oacute;n:</strong> Sarasa XXXX B&ordm; Sarlanga, C&oacute;rdoba Capital</p>");
    header.append("<p style='text-align: justify;'><strong>Tel.</strong>: 0351-4123156</p>");
    header.append("<p style='text-align: justify;'><strong>Cel.</strong>: 0351-150123456</p>");
    header.append("<hr />");
    header.append("<h2 style='color: #2e6c80; text-align: center;'>Actividad de Empleado</h2>");
    //header.append("<h3 style='color: #2e6c80; text-align: center;'>" + mUi->dateTimeEdit_Desde->text() + " - " + mUi->dateTimeEdit_Hasta->text() + "</h3>");

    fullHtmlCode.append(header);

    QString htmlString;
    // INGRESOS/ENTREGAS
    {
        // Header
        htmlString.append("<table width = '100%'>");
        htmlString.append("<u><thead><tr>");
        htmlString.append("<th align = center valign = middle>DNI</th>");
        htmlString.append("<th align = center valign = middle>Nombre</th>");
        htmlString.append("</tr></thead></u>");

        // Data
        for (int ID = 1; ID <= getTableEntriesCount(TableNames::OPERARIOS).value(); ++ID) {
            htmlString.append("<tr>");

            std::vector<KeyAndValue> condition = {KeyAndValue(OperariosFields::ID, QString::number(ID))};
            Result<std::vector<KeyAndValue>> selectRes = select(TableNames::OPERARIOS, "*", condition);
            if(selectRes.status() == Status::FAILED) {
                ERROR("ERROR");
                throw;
            }
            std::vector<KeyAndValue> data = selectRes.value();

            for (KeyAndValue keyAndValue : data) {
                if (keyAndValue.mKey == OperariosFields::DNI) {
                    htmlString.append(QString("<td align = center valign = middle>%1</th>").arg(keyAndValue.mValue));
                }
                if (keyAndValue.mKey == OperariosFields::NOMBRE_COMPLETO) {
                    htmlString.append(QString("<td align = center valign = middle>%1</td>").arg(keyAndValue.mValue));
                }
            }
            htmlString.append("</tr>\n");
        }
        htmlString.append("</table><br />");
    }

    fullHtmlCode.append(htmlString);

    fullHtmlCode.append("</body></html>");

    mPrintPreview->setText(fullHtmlCode);
    mPrintPreview->show();
}
