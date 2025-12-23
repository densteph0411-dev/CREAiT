#ifndef AUTOMATIONSVIEWPAGE_H
#define AUTOMATIONSVIEWPAGE_H

#include <QWidget>
#include <QGridLayout>
#include <QMap>
#include "ui_automationsviewpage.h"
#include <automationcell.h>
#include <automationdatacontroller.h>

class AutomationsViewPage : public QWidget, public Ui::AutomationsViewPage
{
    Q_OBJECT

public:
    explicit AutomationsViewPage(QWidget *parent = nullptr);
    ~AutomationsViewPage();

    void initializeWidgets();

    void setAutomationWidgets();

    void refresh();

signals:
    void requestCreatePage(int pageId, const QString& cellId);
    void requestDetailPage(int pageId, const QString& cellId);
    void requestAutomationDelete(int pageId, const QString& cellId);

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    void createAutomation();
    void editAutomation(int pageId, const QString& cellId);
    void deleteAutomation(int pageId, const QString& cellId);

private:
    QMap<QString, AutomationData> automations;
    QMap<QString, AutomationCell*> automationWidgets;
    QGridLayout* containerLayout;
};

#endif // AUTOMATIONSVIEWPAGE_H
