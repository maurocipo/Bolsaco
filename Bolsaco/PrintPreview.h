#ifndef PRINTPREVIEW_H
#define PRINTPREVIEW_H

#include <QWidget>

namespace Ui {
class PrintPreview;
}

class PrintPreview : public QWidget
{
    Q_OBJECT

public:
    explicit PrintPreview(QWidget *parent = nullptr);
    ~PrintPreview();

    void setText(const QString& aHTMLText);

private:
    Ui::PrintPreview *mUi;
};

#endif // PRINTPREVIEW_H
