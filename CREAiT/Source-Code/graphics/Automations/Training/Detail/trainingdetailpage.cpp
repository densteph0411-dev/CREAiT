#include "trainingdetailpage.h"
#include <automationdatacontroller.h>

TrainingDetailPage::TrainingDetailPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    containerLayout->setAlignment(Qt::AlignTop);

    connect(backButton, &QPushButton::clicked, this, &TrainingDetailPage::clickedBackButton);
    connect(editButton, &QPushButton::clicked, this, &TrainingDetailPage::clickedEditButton);
}

TrainingDetailPage::~TrainingDetailPage() {

}

void TrainingDetailPage::setTrainingDetailById(const QString& id) {
    trainingId = id;

    AutomationData automation = AutomationDataController::loadRecordById(trainingId, "training");

    backButton->setText(automation.title);
    mainPanel->setTrainingDetail(automation.AIModel, automation.lastModifiedOn, automation.lastModifiedBy, automation.prompt);
}


void TrainingDetailPage::clickedBackButton() {
    emit requestGoBack(1);
}

void TrainingDetailPage::clickedEditButton() {
    emit requestEdit(1, trainingId);
}
