#include "automationcreatepage.h"
#include <QLineEdit>
#include <QDebug>
#include <automationdatacontroller.h>

AutomationCreatePage::AutomationCreatePage(QWidget *parent)
    : QWidget(parent), automationId("")
{
    setupUi(this);

    initializeWidgets();

    connect(triggerTypeSelect, &QComboBox::currentTextChanged, this, &AutomationCreatePage::changedTriggerValue);
    connect(backButton, &QPushButton::clicked, this, &AutomationCreatePage::clickedNavigationBack);
    connect(saveButton, &QPushButton::clicked, this, &AutomationCreatePage::clickedAutomationSave);
    connect(cancelButton, &QPushButton::clicked, this, &AutomationCreatePage::clickedAutomationCancel);
}

AutomationCreatePage::~AutomationCreatePage() {

}

void AutomationCreatePage::initializeWidgets() {
    triggerComboValues.append("Record Created");
    triggerComboValues.append("File upload");
    triggerComboValues.append("Review created");
    triggerComboValues.append("Periodic");

    periodicUnitComboValues.append("Seconds");
    periodicUnitComboValues.append("Minutes");
    periodicUnitComboValues.append("Hours");
    periodicUnitComboValues.append("Days");
    periodicUnitComboValues.append("Weeks");
    periodicUnitComboValues.append("Months");
    periodicUnitComboValues.append("Years");

    mainLayout->setAlignment(Qt::AlignTop);
    aiModelSelect->lineEdit()->setPlaceholderText(tr("Select AI model here"));
    versionSelect->lineEdit()->setPlaceholderText("Select version here");
    triggerTypeSelect->lineEdit()->setPlaceholderText("Select trigger here");

    aiModelSelect->addItems({ "OpenAI | gpt-turbo-3.6", "OpenAI | gpt-turbo-3.7", "OpenAI | gpt-turbo-3.9" });
    versionSelect->addItems({
        "2.0.6.13",
        "2.0.6.1",
        "2.0.6.4",
        "2.0.6.7",
        "2.0.6.19",
    });

    periodicPropertyContainer->hide();

    for (auto item: triggerComboValues) {
        triggerTypeSelect->addItem(item);
    }

    for (auto item: periodicUnitComboValues)
        periodicUnitSelect->addItem(item);
}

void AutomationCreatePage::setAutomationDataById(const QString& id) {
    automationId = id;

    AutomationData automation = AutomationDataController::loadRecordById(automationId);

    backButton->setText(automation.title);
    titleEdit->setText(automation.title);
    aiModelSelect->setCurrentText(automation.AIModel);
    versionSelect->setCurrentText(automation.version);
    promptEdit->setText(automation.prompt);
    triggerTypeSelect->setCurrentText(automation.triggerType);
    periodicUnitSelect->setCurrentText(automation.durationType);
    periodicValueEdit->setText(automation.duration);

}

void AutomationCreatePage::changedTriggerValue(const QString& value) {
    if (value == triggerComboValues.at(3))
        periodicPropertyContainer->show();
    else
        periodicPropertyContainer->hide();
}

void AutomationCreatePage::clickedNavigationBack() {
    emit requestGoBackPage(0);
}

void AutomationCreatePage::clickedAutomationSave() {
    AutomationData automation;
    automation.recordID = automationId;
    automation.type = "automation";
    automation.title = titleEdit->text();
    automation.prompt = promptEdit->toPlainText();
    automation.AIModel = aiModelSelect->currentText();
    automation.version = versionSelect->currentText();
    automation.triggerType = triggerTypeSelect->currentText();
    automation.durationType = periodicUnitSelect->currentText();
    automation.duration = periodicValueEdit->text();

    QString recordId;
    if (automationId.isEmpty()) {
        recordId = AutomationDataController::addRecord(automation);
        emit requestDetailPage(0, recordId);
    } else {
        AutomationDataController::updateRecord(automation);
        emit requestGoBackPage(0);
    }
}

void AutomationCreatePage::clickedAutomationCancel() {
    emit requestGoBackPage(0);
}
