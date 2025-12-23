#ifndef AUTOMATIONSSIDEBAR_H
#define AUTOMATIONSSIDEBAR_H

#include <QWidget>

#include <styleSheet.h>
#include "ui_automationssidebar.h"

class AutomationsSideBar : public QWidget, public Ui::AutomationsSideBar, public StyleSheet
{
    Q_OBJECT

public:
    explicit AutomationsSideBar(QWidget *parent = nullptr);
    ~AutomationsSideBar();

signals:
    void requestedPage(int pageIndex);

public slots:
    void handleButtonSelect(int buttonIndex);

private:
    QList<QToolButton *>* pageButtons;
};

#endif // AUTOMATIONSSIDEBAR_H
