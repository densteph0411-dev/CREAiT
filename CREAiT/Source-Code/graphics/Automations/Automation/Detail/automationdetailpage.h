#ifndef AUTOMATIONDETAILPAGE_H
#define AUTOMATIONDETAILPAGE_H

#include <QWidget>
#include <QPaintEvent>
#include <styleSheet.h>
#include "ui_automationdetailpage.h"
#include <automationtestresultdialog.h>

class AutomationDetailPage : public QWidget, public StyleSheet, public Ui::AutomationDetailPage
{
    Q_OBJECT

public:
    explicit AutomationDetailPage(QWidget *parent = nullptr);
    ~AutomationDetailPage();

    void setAutomationDetailById(const QString& id);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void requestGoBackPage(int pageIndex);
    void requestEdit(int pageId, const QString& automationId);

private slots:
    void clickedBackButton();
    void clickedEditButton();
    void clickedTestButton();

private:
    QString automationId;
    AutomationTestResultDialog* testResultDialog;
};

#endif // AUTOMATIONDETAILPAGE_H
