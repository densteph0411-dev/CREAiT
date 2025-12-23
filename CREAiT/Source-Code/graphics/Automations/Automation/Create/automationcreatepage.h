#ifndef AUTOMATIONCREATEPAGE_H
#define AUTOMATIONCREATEPAGE_H

#include <QWidget>
// #include <QStringList>
#include "ui_automationcreatepage.h"


class AutomationCreatePage : public QWidget, public Ui::AutomationCreatePage
{
    Q_OBJECT

public:
    explicit AutomationCreatePage(QWidget *parent = nullptr);
    ~AutomationCreatePage();

    void initializeWidgets();

    void setType(int type = 0);
    void setAutomationDataById(const QString& id = "");
signals:
    void requestGoBackPage(int pageIndex);
    void requestDetailPage(int pageId, const QString& cellId);
    void requestAutomationSave();
    void requestAutomationCancel();

public slots:
    void changedTriggerValue(const QString& value);
    void clickedNavigationBack();
    void clickedAutomationSave();
    void clickedAutomationCancel();

private:
    QString automationId;
    QStringList triggerComboValues;
    QStringList periodicUnitComboValues;
};

#endif // AUTOMATIONCREATEPAGE_H
