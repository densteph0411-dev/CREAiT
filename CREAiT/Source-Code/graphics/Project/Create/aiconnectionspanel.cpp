#include "aiconnectionspanel.h"
#include "records.h"   // <-- add this

AIConnectionsPanel::AIConnectionsPanel(QWidget *parent)
    : QWidget(parent), nextId(0)
{
    setupUi(this);

    initializeWidgets();

    connect(addButton, &QPushButton::clicked, this, &AIConnectionsPanel::clickedAddButton);
    connect(backButton, &QPushButton::clicked, this, &AIConnectionsPanel::clickedBackButton);
    connect(nextButton, &QPushButton::clicked, this, &AIConnectionsPanel::clickedNextButton);
}

AIConnectionsPanel::~AIConnectionsPanel()
{

}

void AIConnectionsPanel::initializeWidgets() {
    mainLayout->setAlignment(Qt::AlignTop);

    aiConnectionPanels = new QMap<int, AIConnectionPanel*>();
}

void AIConnectionsPanel::clickedAddButton() {
    int id = nextId++;
    auto panel = new AIConnectionPanel(id, this);
    aiConnectionPanels->insert(id, panel);
    aiConnectionLayout->addWidget(panel);

    connect(panel, &AIConnectionPanel::requestDelete, this, &AIConnectionsPanel::handleDelete);
}

void AIConnectionsPanel::handleDelete(int id) {
    if (aiConnectionPanels->size() <= 1) return;

    if (aiConnectionPanels->contains(id)) {
        auto* panel = aiConnectionPanels->take(id);
        aiConnectionLayout->removeWidget(panel);
        panel->deleteLater();
    }
}

void AIConnectionsPanel::clickedBackButton() {
    emit requestBack();
}

void AIConnectionsPanel::clickedNextButton()
{
    ConnectionsAndIntegrations connectionManager;

    QMapIterator<int, AIConnectionPanel*> it(*aiConnectionPanels);
    while (it.hasNext()) {
        it.next();
        AIConnectionPanel* panel = it.value();
        if (!panel)
            continue;

        const QString aiTool = panel->aiSelect->currentText().trimmed();
        const QString model  = panel->modelEdit->text().trimmed();
        const QString apiKey = panel->apiKeyEdit->text().trimmed();
        const QString url    = getProviderURL(aiTool);   // your helper

        // Skip if ANY required piece is missing
        if (aiTool.isEmpty() || model.isEmpty() || apiKey.isEmpty() || url.isEmpty())
            continue;

        connectionManager.clearDictionaryRecordData();

        const std::string newId = connectionManager.create(
            "AIConnection",
            aiTool.toStdString(),
            model.toStdString(),
            apiKey.toStdString()
            );

        Data::setDataValue("type",           "connection", "AIConnection");
        Data::setDataValue("ToolName",       "connection", aiTool.toStdString());
        Data::setDataValue("url",            "connection", url.toStdString());
        Data::setDataValue("ModelOrVersion", "connection", model.toStdString());
        Data::setDataValue("Key",            "connection", apiKey.toStdString());

        createdAIConnectionIDs.append(QString::fromStdString(newId));

        connectionManager.clearDictionaryRecordData();
    }

    emit requestNext();
}

void AIConnectionsPanel::reset() {
    QMapIterator<int, AIConnectionPanel*> it(*aiConnectionPanels);
    while (it.hasNext()) {
        it.next();
        AIConnectionPanel *panel = it.value();
        aiConnectionLayout->removeWidget(panel);
        panel->deleteLater();
    }
    aiConnectionPanels->clear();
    
    nextId = 0;
    clickedAddButton();
}

QString AIConnectionsPanel::getProviderURL(const QString& aiTool)
{
    const QString tool = aiTool.trimmed().toLower();

    // --- Known provider → URL mapping ---
    if (tool == "openai") {
        return "https://api.openai.com/v1";
    }
    if (tool == "azure openai" || tool == "azure") {
        // Azure uses custom deployment endpoints; placeholder:
        return "https://{your-resource-name}.openai.azure.com/openai";
    }
    if (tool == "anthropic" || tool == "claude") {
        return "https://api.anthropic.com/v1";
    }
    if (tool == "gemini" || tool == "google gemini" || tool == "google") {
        return "https://generativelanguage.googleapis.com/v1";
    }
    if (tool == "ollama") {
        return "http://localhost:11434";
    }
    if (tool == "local" || tool == "custom") {
        return "http://localhost";
    }
    if (tool == "ace") {
        // Replace if your ACE provider uses another endpoint
        return "https://ace-api.example.com/v1";
    }

    // Unknown provider — return an empty string
    return "";
}

void AIConnectionsPanel::removeCreatedAIConnections()
{
    if (createdAIConnectionIDs.isEmpty())
        return;

    ConnectionsAndIntegrations connectionManager;

    for (const QString& id : std::as_const(createdAIConnectionIDs)) {
        connectionManager.remove(id.toStdString());
    }

    createdAIConnectionIDs.clear();
}
