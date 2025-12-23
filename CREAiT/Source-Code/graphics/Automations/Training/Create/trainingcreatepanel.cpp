#include "trainingcreatepanel.h"
#include <QVBoxLayout>

TrainingCreatePanel::TrainingCreatePanel(QWidget *parent)
    : QWidget(parent), nextId(1)
{
    setupUi(this);

    datasetContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    datasetLayout = new QVBoxLayout(datasetContainer);
    datasetLayout->setContentsMargins(0, 0, 0, 0);
    datasetLayout->setSpacing(4);
    datasetLayout->setAlignment(Qt::AlignTop);

    initializeWidgets();

    emit addDataset();

    connect(addDatasetButton, &QPushButton::clicked, this, &TrainingCreatePanel::addDataset);
}

TrainingCreatePanel::~TrainingCreatePanel()
{

}

void TrainingCreatePanel::initializeWidgets() {
    aiModelSelect->addItems({
        "Open AI | gpt-turbo-3.6",
        "Gemini | 1.5 Pro",
        "Open AI | gpt-turbo-3.7",
        "Gemini | 1.5 Flash",
        "Open AI | gpt-turbo-3.9",
    });

    versionSelect->addItems({
        "2.0.6.13",
        "2.0.6.1",
        "2.0.6.4",
        "2.0.6.7",
        "2.0.6.19",
    });
}

void TrainingCreatePanel::addDataset() {
    int id = nextId++;
    auto* dataset = new AccordionWidget(id, QString("Dataset %1").arg(id), this);
    datasets[id] = dataset;
    datasetLayout->addWidget(dataset);

    connect(dataset, &AccordionWidget::requestDelelte, this, &TrainingCreatePanel::deleteDataset);
}


void TrainingCreatePanel::deleteDataset(int id) {
    if (datasets.size() <= 1) return;

    if (datasets.contains(id)) {
        AccordionWidget *dataset = datasets.take(id);
        datasetLayout->removeWidget(dataset);
        dataset->deleteLater();
    }
}

