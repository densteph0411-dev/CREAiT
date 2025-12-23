#ifndef HSEPARATORWIDGET_H
#define HSEPARATORWIDGET_H

#include <QWidget>

namespace Ui {
class HSeparatorWidget;
}

class HSeparatorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HSeparatorWidget(QWidget *parent = nullptr);
    ~HSeparatorWidget();

private:
    Ui::HSeparatorWidget *ui;
};

#endif // HSEPARATORWIDGET_H
