#include "aisettingspage.h"
#include "aiconnectionaddeditdialog.h"
#include <records.h>

AISettingsPage::AISettingsPage(QWidget *parent)
    : QWidget(parent), deleteId(-1), dataInitialized(false)
{
    setupUi(this);

    containerLayout = new QGridLayout(container);
    containerLayout->setSpacing(18);
    containerLayout->setContentsMargins(0, 0, 10, 0);
    containerLayout->setAlignment(Qt::AlignTop);

    aiConnections = new QList<AIConnection*>();

    deleteDialog = new DeleteDialog();

    connect(createButton, &QPushButton::clicked, this, &AISettingsPage::clickedCreateAIConnectionButton);
    connect(deleteDialog, &DeleteDialog::requestDelete, this, &AISettingsPage::handleAIConnectionDelete);
}

AISettingsPage::~AISettingsPage() {
    qDeleteAll(*aiConnections);
    aiConnections->clear();
}

void AISettingsPage::setAIConnections() {
    QLayoutItem *item;
    while ((item = containerLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    const int columns = 3;
    int row = 0, col = 0;

    for (int i = 0; i < aiConnections->size(); ++i) {
        containerLayout->addWidget(aiConnections->at(i), row, col);
        col++;
        if (col >= columns) {
            col = 0;
            row++;
        }
    }
}

void AISettingsPage::refresh() {
    if (Data::getDataValue("projectName", "project").empty())
        return;

    qDeleteAll(*aiConnections);
    aiConnections->clear();

    aiConnectionData = ProjectSettingsController::getAllAIConnections();
    for (auto id: aiConnectionData.keys()) {
        auto *cell = new AIConnection(1, id);
        cell->headerLabel->setText(aiConnectionData[id].ID.left(18) + "...");
        cell->urlPanel->icon->setIcon(QIcon(":/images/ProjectSettings/url.svg"));
        cell->urlPanel->titleLabel->setText("URL");
        cell->urlPanel->description->setText(aiConnectionData[id].url);
        //cell->headerLabel->setText(aiConnectionData[id].id);
        QString ai = QString("%1 | %2").arg(aiConnectionData[id].aiModel, aiConnectionData[id].version);
        cell->aiModelPanel->icon->setIcon(QIcon(":/images/ProjectSettings/ai_model.svg"));
        cell->aiModelPanel->titleLabel->setText("AI Model");
        cell->aiModelPanel->description->setText(ai);


        connect(cell, &AIConnection::requestRefresh, this, &AISettingsPage::refreshAIConnection);
        connect(cell, &AIConnection::requestEdit, this, &AISettingsPage::editAIConnection);
        connect(cell, &AIConnection::requestDelete, this, &AISettingsPage::deleteAIConnection);

        aiConnections->append(cell);
    }
    setAIConnections();
}

void AISettingsPage::clickedCreateAIConnectionButton() {
    AIConnectionAddEditDialog* dialog = new AIConnectionAddEditDialog();
    dialog->show();

    connect(dialog, &AIConnectionAddEditDialog::requestRefreshAll, this, &AISettingsPage::refresh);
}

void AISettingsPage::refreshAIConnection(int pageId, int cellId) {
    if (!aiConnectionData.contains(cellId))
        return;

    AIConnectionData updatedData = ProjectSettingsController::getAIConnectionById(cellId);
    aiConnectionData[cellId] = updatedData;

    for (AIConnection* cell : *aiConnections) {
        if (cell->getCellId() == cellId) {
            QString ai = QString("%1 | %2").arg(aiConnectionData[cellId].aiModel).arg(aiConnectionData[cellId].version);
            cell->urlPanel->description->setText(updatedData.url);
            cell->aiModelPanel->description->setText(ai);
            break;
        }
    }
}

void AISettingsPage::editAIConnection(int pageId, int cellId) {
    AIConnectionAddEditDialog* dialog = new AIConnectionAddEditDialog(1);
    dialog->setData(cellId);
    dialog->show();

    connect(dialog, &AIConnectionAddEditDialog::requestRefresh, this, &AISettingsPage::refreshAIConnection);
}

void AISettingsPage::deleteAIConnection(int pageId, int cellId) {
    deleteDialog->show();
    deleteId = cellId;
    connect(deleteDialog, &DeleteDialog::requestDelete, this, &AISettingsPage::handleAIConnectionDelete);
}

void AISettingsPage::handleAIConnectionDelete() {
    if (deleteId == -1) return;

    ProjectSettingsController::deleteAIConnectionById(deleteId);
    refresh();
    deleteId = -1;

}

void AISettingsPage::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    std::string projectNameStd = Data::getDataValue("projectName", "project");
    if (projectNameStd.empty())
        return;

    QString projectName = QString::fromStdString(projectNameStd);
    if (!dataInitialized || loadedProjectName != projectName) {
        refresh();
        dataInitialized = true;
        loadedProjectName = projectName;
    }
}

