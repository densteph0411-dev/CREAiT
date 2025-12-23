#include "automationdetailpage.h"

#include <QIcon>
#include <QPainter>
#include <QStyleOption>
#include <automationdatacontroller.h>

AutomationDetailPage::AutomationDetailPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    connect(backButton, &QPushButton::clicked, this, &AutomationDetailPage::clickedBackButton);
    connect(editAutomationButton, &QPushButton::clicked, this, &AutomationDetailPage::clickedEditButton);
    connect(testAutomationButton, &QPushButton::clicked, this, &AutomationDetailPage::clickedTestButton);
}

AutomationDetailPage::~AutomationDetailPage()
{
}

void AutomationDetailPage::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void AutomationDetailPage::setAutomationDetailById(const QString &id)
{
    automationId = id;

    AutomationData automation = AutomationDataController::loadRecordById(automationId);

    backButton->setText(automation.title);

    aiModel->titleLabel->setText("AI Model");
    aiModel->icon->setIcon(QIcon(":/images/Automations/aimodel.png"));
    aiModel->description->setText(automation.AIModel);

    version->titleLabel->setText("Version");
    version->icon->setIcon(QIcon(":/images/Automations/version.png"));
    version->description->setText(automation.version);

    trigger->titleLabel->setText("Trigger");
    trigger->icon->setIcon(QIcon(":/images/Automations/trigger.png"));
    trigger->description->setText(automation.triggerType);
    
    QString html = QString(R"(
        <div style="font-family: Open Sans; font-size: 18px; line-height: 1.2;">
            %1
        </div>
    )").arg(automation.prompt.toHtmlEscaped());

    promptBrowser->setHtml(html);
    promptBrowser->setReadOnly(true);
    promptBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    promptBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void AutomationDetailPage::clickedBackButton() {
    emit requestGoBackPage(0);
}

void AutomationDetailPage::clickedEditButton() {
    emit requestEdit(0, automationId);
}

void AutomationDetailPage::clickedTestButton() {
    testResultDialog = new AutomationTestResultDialog(this);
    testResultDialog->show();
}
