#include "integrationspanel.h"
#include "records.h"   // <-- add this

IntegrationsPanel::IntegrationsPanel(QWidget *parent)
    : QWidget(parent), nextId(0)
{
    setupUi(this);

    initializeWidgets();

    connect(addButton, &QPushButton::clicked, this, &IntegrationsPanel::clickedAddButton);
    connect(backButton, &QPushButton::clicked, this, &IntegrationsPanel::clickedBackButton);
    connect(nextButton, &QPushButton::clicked, this, &IntegrationsPanel::clickedNextButton);
}

IntegrationsPanel::~IntegrationsPanel()
{

}

void IntegrationsPanel::initializeWidgets() {
    mainLayout->setAlignment(Qt::AlignTop);

    integrationPanels = new QMap<int, IntegrationPanel*>();
}

void IntegrationsPanel::clickedAddButton() {
    int id = nextId++;
    auto* panel = new IntegrationPanel(id, this);
    integrationPanels->insert(id, panel);
    integrationLayout->addWidget(panel);

    connect(panel, &IntegrationPanel::requestDelete, this, &IntegrationsPanel::handleDelete);
}

void IntegrationsPanel::handleDelete(int id) {
    if (integrationPanels->size() <= 1) return;

    if (integrationPanels->contains(id)) {
        auto* panel = integrationPanels->take(id);
        integrationLayout->removeWidget(panel);
        panel->deleteLater();
    }
}

void IntegrationsPanel::clickedBackButton() {
    emit requestBack();
}

void IntegrationsPanel::clickedNextButton()
{
    ConnectionsAndIntegrations connectionManager;

    QMapIterator<int, IntegrationPanel*> it(*integrationPanels);
    while (it.hasNext()) {
        it.next();
        IntegrationPanel* panel = it.value();
        if (!panel)
            continue;

        const QString tool     = panel->toolSelect->currentText().trimmed();
        const QString url      = panel->urlEdit->text().trimmed();
        const QString username = panel->usernameEdit->text().trimmed();
        const QString password = panel->passwordEdit->text().trimmed();
        const QString apiKey   = panel->apiKeyEdit->text().trimmed();

        // Skip if ANY required field is empty:
        // required: url, username, and (password OR apiKey)
        if (url.isEmpty() ||
            username.isEmpty() ||
            (password.isEmpty() && apiKey.isEmpty()))
        {
            continue;
        }

        const std::string key = !apiKey.isEmpty()
                                    ? apiKey.toStdString()
                                    : password.toStdString();

        connectionManager.clearDictionaryRecordData();

        Data::setDataValue("type",           "connection", "Integration");
        Data::setDataValue("ToolName",       "connection", tool.toStdString());
        Data::setDataValue("url",            "connection", url.toStdString());
        Data::setDataValue("ModelOrVersion", "connection", username.toStdString());
        Data::setDataValue("Key",            "connection", key);

        // IMPORTANT: capture the ID we get back
        const std::string newId = connectionManager.create(
            "Integration",
            tool.toStdString(),
            username.toStdString(),
            key
            );

        createdIntegrationIDs.append(QString::fromStdString(newId));

        connectionManager.clearDictionaryRecordData();
    }

    emit requestNext();
}

void IntegrationsPanel::reset() {
    QMapIterator<int, IntegrationPanel*> it(*integrationPanels);
    while (it.hasNext()) {
        it.next();
        IntegrationPanel *panel = it.value();
        integrationLayout->removeWidget(panel);
        panel->deleteLater();
    }
    integrationPanels->clear();
    
    nextId = 0;
    clickedAddButton();
}

void IntegrationsPanel::removeCreatedIntegrations()
{
    if (createdIntegrationIDs.isEmpty())
        return;

    ConnectionsAndIntegrations connectionManager;

    for (const QString& id : std::as_const(createdIntegrationIDs)) {
        connectionManager.remove(id.toStdString());
    }

    createdIntegrationIDs.clear();
}
