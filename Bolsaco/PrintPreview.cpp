#include "PrintPreview.h"
#include "ui_PrintPreview.h"

PrintPreview::PrintPreview(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::PrintPreview)
{
    mUi->setupUi(this);
}

PrintPreview::~PrintPreview()
{
    delete mUi;
}

void
PrintPreview::setText(const QString& aHTMLText)
{
    mUi->textBrowser->setHtml(aHTMLText);
}
