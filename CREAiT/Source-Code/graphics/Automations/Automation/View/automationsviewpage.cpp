#include "automationsviewpage.h"
#include <QIcon>
#include <QLineEdit>
#include <QLayoutItem>

AutomationsViewPage::AutomationsViewPage(QWidget *parent)
    : QWidget(parent)
{
    initializeWidgets();
}

AutomationsViewPage::~AutomationsViewPage() {
    qDeleteAll(automationWidgets);
    automations.clear();
    automationWidgets.clear();
}

void AutomationsViewPage::initializeWidgets() {
    setupUi(this);

    aiModelSelect->lineEdit()->setPlaceholderText(tr("Select AI Model"));
    triggerTypeSelect->lineEdit()->setPlaceholderText(tr("Select Trigger Type"));

    containerLayout = new QGridLayout(container);
    containerLayout->setSpacing(18);
    containerLayout->setContentsMargins(0, 0, 10, 0);
    containerLayout->setAlignment(Qt::AlignTop);

    connect(createAutomationButton, &QPushButton::clicked, this, &AutomationsViewPage::createAutomation);
}

void AutomationsViewPage::setAutomationWidgets() {
    QLayoutItem *item;
    while ((item = containerLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    const int columns = 3;
    int row = 0, col = 0;

    for (const QString& key: automationWidgets.keys()) {
        AutomationCell* cell = automationWidgets.value(key);
        if (cell) {
            containerLayout->addWidget(cell, row, col);
            col++;
            if (col >= columns) {
                col = 0;
                row++;
            }
        }
    }
}

void AutomationsViewPage::refresh() {
    qDeleteAll(automationWidgets);
    automations.clear();
    automationWidgets.clear();

    automations = AutomationDataController::loadAllRecords("automation");

    for (auto automation: automations) {
        if (automation.type == "training" || automation.deleted == "true")
            continue;

        AutomationCell* cell = new AutomationCell(0, automation.recordID);
        cell->titleLabel->setText(automation.title);
        cell->descriptionLabel->setText(automation.description);
        cell->leftPanel->icon->setIcon(QIcon(":/images/Automations/cell_footer_aimodel.svg"));
        cell->leftPanel->titleLabel->setText("AI Model");
        cell->leftPanel->description->setText(automation.AIModel);
        cell->middlePanel->icon->setIcon(QIcon(":/images/Automations/cell_footer_version.svg"));
        cell->middlePanel->titleLabel->setText("Version");
        cell->middlePanel->description->setText(automation.version);
        cell->rightPanel->icon->setIcon(QIcon(":/images/Automations/cell_footer_trigger.svg"));
        cell->rightPanel->titleLabel->setText("Trigger");
        cell->rightPanel->description->setText(automation.triggerType);

        connect(cell, &AutomationCell::requestEdit, this, &AutomationsViewPage::editAutomation);
        connect(cell, &AutomationCell::requestDelete, this, &AutomationsViewPage::deleteAutomation);

        automationWidgets.insert(automation.recordID, cell);
    }

    setAutomationWidgets();
}

void AutomationsViewPage::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    refresh();
}

void AutomationsViewPage::createAutomation() {
    emit requestCreatePage(2, "");
}

void AutomationsViewPage::editAutomation(int pageId, const QString& cellId) {
    if (pageId < 0 || pageId > 1) {
        qDebug() << "Invalid page id to edit";
        return;
    }
    if (cellId.isEmpty()) {
        qDebug() << "Invalid cell id to edit";
        return;
    }
    emit requestDetailPage(pageId, cellId);
}

void AutomationsViewPage::deleteAutomation(int pageId, const QString& cellId) {
    if (pageId < 0 || pageId > 1) {
        qDebug() << "Invalid page id to delete";
        return;
    }
    if (cellId.isEmpty()) {
        qDebug() << "Invalid cell id to delete";
        return;
    }

    AutomationDataController::deleteRecordById(cellId);

    refresh();
}
