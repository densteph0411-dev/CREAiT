#include "integrationspage.h"
#include <QLayoutItem>
#include "integrationaddeditdialog.h"
#include "deletedialog.h"
#include <records.h>

IntegrationsPage::IntegrationsPage(QWidget *parent)
    : QWidget(parent), deleteId(-1), dataInitialized(false)
{
    setupUi(this);

    containerLayout = new QGridLayout(container);
    containerLayout->setSpacing(18);
    containerLayout->setContentsMargins(0, 0, 10, 0);
    containerLayout->setAlignment(Qt::AlignTop);

    integrations = new QList<Integration*>();

    deleteDialog = new DeleteDialog();

    connect(createButton, &QPushButton::clicked, this, &IntegrationsPage::clickedCreateIntegrationButton);
    connect(deleteDialog, &DeleteDialog::requestDelete, this, &IntegrationsPage::handleIntegrationDelete);
}

IntegrationsPage::~IntegrationsPage() {
    qDeleteAll(*integrations);
    delete integrations;
}

Integration* IntegrationsPage::createIntegrationCell(int id)
{
    IntegrationData& data = integrationData[id];

    auto *cell = new Integration(1, id);

    // Set the Title of the panel
    cell->headerLabel->setText(data.title);

    cell->urlPanel->icon->setIcon(QIcon(":/images/ProjectSettings/url.svg"));
    cell->urlPanel->titleLabel->setText("URL");
    cell->urlPanel->description->setText(data.url);

    cell->apiKeyPanel->icon->setIcon(QIcon(":/images/ProjectSettings/api_key.svg"));
    cell->apiKeyPanel->titleLabel->setText("API Key");
    cell->apiKeyPanel->description->setText(data.Key);

    cell->usernamePanel->icon->setIcon(QIcon(":/images/ProjectSettings/avatar.svg"));
    cell->usernamePanel->titleLabel->setText("Username");
    cell->usernamePanel->description->setText(data.username);

    cell->passwordPanel->icon->setIcon(QIcon(":/images/ProjectSettings/password.svg"));
    cell->passwordPanel->titleLabel->setText("Password");
    cell->passwordPanel->description->setText(data.password);

    cell->connectedSincePanel->icon->setIcon(QIcon(":/images/ProjectSettings/date_at.svg"));
    cell->connectedSincePanel->titleLabel->setText("Connected Since");
    cell->connectedSincePanel->description->setText(data.connectionDate);

    connect(cell, &Integration::requestRefresh, this, &IntegrationsPage::refreshIntegration);
    connect(cell, &Integration::requestEdit, this, &IntegrationsPage::editIntegration);
    connect(cell, &Integration::requestDelete, this, &IntegrationsPage::deleteIntegration);

    return cell;
}

void IntegrationsPage::setIntegrations() {
    QLayoutItem *item;
    while ((item = containerLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    const int columns = 3;
    int row = 0, col = 0;

    for (int i = 0; i < integrations->size(); ++i) {
        containerLayout->addWidget(integrations->at(i), row, col);
        col++;
        if (col >= columns) {
            col = 0;
            row++;
        }
    }
}

void IntegrationsPage::refresh() {
    if (Data::getDataValue("projectName", "project").empty())
        return;

    qDeleteAll(*integrations);
    integrations->clear();

    integrationData = ProjectSettingsController::getAllIntegrations();

    for (auto id : integrationData.keys()) {

        integrations->append(createIntegrationCell(id));
    }

    setIntegrations();
}

void IntegrationsPage::clickedCreateIntegrationButton() {
    IntegrationAddEditDialog* dialog = new IntegrationAddEditDialog();
    dialog->show();
    connect(dialog, &IntegrationAddEditDialog::requestRefreshAll, this, &IntegrationsPage::refresh);
}

void IntegrationsPage::refreshIntegration(int pageId, int cellId) {
    if (!integrationData.contains(cellId))
        return;

    IntegrationData updatedData = ProjectSettingsController::getIntegrationById(cellId);
    integrationData[cellId] = updatedData;

    for (Integration* cell : *integrations) {
        if (cell->getCellId() == cellId) {
            cell->urlPanel->description->setText(updatedData.url);
            cell->apiKeyPanel->description->setText(updatedData.Key);
            cell->usernamePanel->description->setText(updatedData.username);
            cell->passwordPanel->description->setText("**********");
            break;
        }
    }
}

void IntegrationsPage::editIntegration(int pageId, int cellId) {
    IntegrationAddEditDialog* dialog = new IntegrationAddEditDialog(1);
    IntegrationData data = ProjectSettingsController::getIntegrationById(cellId);
    dialog->setData(cellId);
    dialog->show();

    connect(dialog, &IntegrationAddEditDialog::requestRefresh, this, &IntegrationsPage::refreshIntegration);
}

void IntegrationsPage::deleteIntegration(int pageId, int cellId) {
    deleteDialog->show();
    deleteId = cellId;
    connect(deleteDialog, &DeleteDialog::requestDelete, this, &IntegrationsPage::handleIntegrationDelete);
}

void IntegrationsPage::handleIntegrationDelete() {
    if (deleteId == -1) return;

    ProjectSettingsController::deleteIntegrationById(deleteId);

    refresh();

    deleteId = -1;
}

void IntegrationsPage::showEvent(QShowEvent *event) {
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
