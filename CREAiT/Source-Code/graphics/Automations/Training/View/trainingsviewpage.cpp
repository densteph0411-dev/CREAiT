#include "trainingsviewpage.h"
#include <QLineEdit>
#include <QLayoutItem>

TrainingsViewPage::TrainingsViewPage(QWidget *parent)
    : QWidget(parent)
{
    initializeWidgets();
}

TrainingsViewPage::~TrainingsViewPage() {

}

void TrainingsViewPage::initializeWidgets() {
    setupUi(this);

    aiModelSelect->lineEdit()->setPlaceholderText(tr("Select AI Model"));

    containerLayout = new QGridLayout(container);
    containerLayout->setSpacing(18);
    containerLayout->setContentsMargins(0, 0, 10, 0);
    containerLayout->setAlignment(Qt::AlignTop);

    connect(trainingCreateButton, &QPushButton::clicked, this, &TrainingsViewPage::createTraining);
}

void TrainingsViewPage::setTrainingWidgets() {
    QLayoutItem *item;
    while ((item = containerLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    const int columns = 3;
    int row = 0, col = 0;
    for (const QString& key: trainingWidgets.keys()) {
        AutomationCell* cell = trainingWidgets.value(key);
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

void TrainingsViewPage::refresh() {
    trainings.clear();
    trainingWidgets.clear();

    trainings = AutomationDataController::loadAllRecords("training");

    for (auto training: trainings) {
        if (training.deleted == "true" || training.type == "automation")
            continue;

        auto *cell = new AutomationCell(1, training.recordID);

        cell->titleLabel->setText(training.title);
        cell->descriptionLabel->setText(training.description);
        cell->leftPanel->icon->setIcon(QIcon(":/images/Automations/cell_footer_aimodel.svg"));
        cell->leftPanel->titleLabel->setText("AI Model");
        cell->leftPanel->description->setText(training.AIModel);

        cell->middlePanel->icon->setIcon(QIcon(":/images/Automations/cell_footer_version.svg"));
        cell->middlePanel->titleLabel->setText("Version");
        cell->middlePanel->description->setText(training.version);

        cell->rightPanel->icon->setIcon(QIcon(":/images/Automations/cell_footer_trigger.svg"));
        cell->rightPanel->titleLabel->setText("Trigger");
        cell->rightPanel->description->setText("File Upload");

        connect(cell, &AutomationCell::requestEdit, this, &TrainingsViewPage::editTraining);
        connect(cell, &AutomationCell::requestDelete, this, &TrainingsViewPage::deleteTraining);

        trainingWidgets.insert(training.recordID, cell);
    }

    setTrainingWidgets();
}

void TrainingsViewPage::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    refresh();
}

void TrainingsViewPage::createTraining() {
    emit requestCreatePage(4, "");
}

void TrainingsViewPage::editTraining(int pageId, const QString& cellId) {
    emit requestDetailPage(pageId, cellId);
}

void TrainingsViewPage::deleteTraining(int pageId, const QString& cellId) {
    AutomationDataController::deleteRecordById(cellId);

    refresh();

    emit requestTrainingDelete(pageId, cellId);
}
