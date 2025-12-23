#ifndef AUTOMATIONCELLFOOTERPANEL_H
#define AUTOMATIONCELLFOOTERPANEL_H

#include <QWidget>
#include "ui_automationcellfooterpanel.h"


class AutomationCellFooterPanel : public QWidget, public Ui::AutomationCellFooterPanel
{
    Q_OBJECT

public:
    explicit AutomationCellFooterPanel(QWidget *parent = nullptr);
    ~AutomationCellFooterPanel();
};

#endif // AUTOMATIONCELLFOOTERPANEL_H
