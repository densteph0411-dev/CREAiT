#include "trainingdetailmainpanel.h"

TrainingDetailMainPanel::TrainingDetailMainPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    setTrainingDetail("Gemini | 1.5 Pro", "March 23, 2025", "John Doe", "");
}

TrainingDetailMainPanel::~TrainingDetailMainPanel()
{

}

void TrainingDetailMainPanel::setTrainingDetail(
    const QString &aiModelText,
    const QString &createdOnText,
    const QString &createdByText,
    const QString &prompt)
{
    aiModel->titleLabel->setText("AI Model");
    aiModel->icon->setIcon(QIcon(":/images/Automations/cell_footer_aimodel.svg"));
    aiModel->description->setText(aiModelText);

    createdOn->titleLabel->setText("Created On");
    createdOn->icon->setIcon(QIcon(":/images/Automations/cell_footer_createdon.svg"));
    createdOn->description->setText(createdOnText);

    createdBy->titleLabel->setText("Created By");
    createdBy->icon->setIcon(QIcon(":/images/Automations/cell_footer_createdby.svg"));
    createdBy->description->setText(createdByText);

    promptLabel->setText(prompt);
}
