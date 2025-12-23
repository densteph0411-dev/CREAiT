#include "trainingcreatepage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QSize>
#include <automationdatacontroller.h>

TrainingCreatePage::TrainingCreatePage(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    containerLayout->setAlignment(Qt::AlignTop);
    initStatusPanel();

    connect(backButton, &QPushButton::clicked, this, &TrainingCreatePage::clickedBackButton);
    connect(startTrainingButton, &QPushButton::clicked, this, &TrainingCreatePage::clickedStartTrainingButton);
    connect(saveButton, &QPushButton::clicked, this, &TrainingCreatePage::clickedSaveButton);
}

TrainingCreatePage::~TrainingCreatePage()
{

}

void TrainingCreatePage::initStatusPanel() {
    QVBoxLayout* statusPanelLayout = new QVBoxLayout(statusPanel);
    statusPanelLayout->setContentsMargins(16, 16, 16, 16);
    QLabel* headerTitleLabel = new QLabel("Training Status", this);
    headerTitleLabel->setObjectName("headerTitleLabel");
    headerTitleLabel->setStyleSheet(R"(
        #headerTitleLabel {
            background: transparent;
            font-family: Open Sans;
            font-size: 24px;
            font-weight: 600;
            line-height: 34px;
            color: #1A1A1A;
        }
    )");

    QPushButton* statusButton = new QPushButton("Not Started", this);
    statusButton->setObjectName("statusButton");
    statusButton->setFixedSize(QSize(125, 34));
    statusButton->setIcon(QIcon(":images/Automations/training_status_not_started.svg"));
    statusButton->setIconSize(QSize(8, 8));
    statusButton->setStyleSheet(R"(
        #statusButton {
            background-color: white;
            border: 2px solid #666666;
            border-radius: 17px;
            font-family: Open Sans;
            font-weight: 500;
            font-size: 14px;
            color: #666666;
        }
    )");

    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->addWidget(headerTitleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(statusButton);

    statusPanelLayout->addLayout(headerLayout);
    statusPanelLayout->addStretch();
}

void TrainingCreatePage::setTrainingDetailById(const QString& id) {
    trainingId = id;

    AutomationData training = AutomationDataController::loadRecordById(trainingId, "training");
    backButton->setText(training.title);
    trainingPanel->titleEdit->setText(training.title);
    trainingPanel->aiModelSelect->setCurrentText(training.AIModel);
    trainingPanel->versionSelect->setCurrentText(training.version);
    trainingPanel->promptEdit->setText(training.prompt);
}

void TrainingCreatePage::clickedBackButton() {
    emit requestGoBackPage(1);
}

void TrainingCreatePage::clickedStartTrainingButton() {

}

void TrainingCreatePage::clickedSaveButton() {
    AutomationData data;

    data.recordID = trainingId;
    data.type = "training";
    data.title = trainingPanel->titleEdit->text();
    data.AIModel = trainingPanel->aiModelSelect->currentText();
    data.version = trainingPanel->versionSelect->currentText();
    data.prompt = trainingPanel->promptEdit->toPlainText();

    if (trainingId.isEmpty()) {
        const QString recordId = AutomationDataController::addRecord(data);
        emit requestDetailPage(1, recordId);
    } else {
        AutomationDataController::updateRecord(data);
        emit requestGoBackPage(1);
    }
}
