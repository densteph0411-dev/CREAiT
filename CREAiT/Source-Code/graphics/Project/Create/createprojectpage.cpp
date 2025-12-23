#include "createprojectpage.h"
#include <QShowEvent>
#include <QScrollBar>
#include <QDebug>
#include <QFileDialog>
#include "records.h"
#include "validationdialog.h"
#include "directoriespanel.h"


CreateProjectPage::CreateProjectPage(QWidget *parent)
    : QWidget(parent), currentIndex(0)
{
    setupUi(this);

    // To get basicInfo.
    if (directoriesPanel && basicInfoPanel)
        directoriesPanel->setBasicInfoPanel(basicInfoPanel);

    connect(cancelButton, &QPushButton::clicked, this, &CreateProjectPage::clickedCancelButton);

    connect(basicInfoPanel, &BasicInfoPanel::requestNext, this, &CreateProjectPage::goNextPanel);
    connect(additionalFieldsPanel, &AdditionalFieldsPanel::requestBack, this, &CreateProjectPage::goBackPanel);
    connect(additionalFieldsPanel, &AdditionalFieldsPanel::requestNext, this, &CreateProjectPage::goNextPanel);
    connect(directoriesPanel, &DirectoriesPanel::requestBack, this, &CreateProjectPage::goBackPanel);
    connect(directoriesPanel, &DirectoriesPanel::requestNext, this, &CreateProjectPage::goNextPanel);
    connect(databaseConfigPanel, &DatabaseConfigurationPanel::requestBack, this, &CreateProjectPage::goBackPanel);
    connect(databaseConfigPanel, &DatabaseConfigurationPanel::requestNext, this, &CreateProjectPage::goNextPanel);
    connect(integrationsPanel, &IntegrationsPanel::requestBack, this, &CreateProjectPage::goBackPanel);
    connect(integrationsPanel, &IntegrationsPanel::requestNext, this, &CreateProjectPage::goNextPanel);
    connect(aiConnectionsPanel, &AIConnectionsPanel::requestBack, this, &CreateProjectPage::goBackPanel);
    connect(aiConnectionsPanel, &AIConnectionsPanel::requestNext, this, &CreateProjectPage::goNextPanel);

    mainPanelContainer->setCurrentIndex(currentIndex);
}

CreateProjectPage::~CreateProjectPage()
{

}

void CreateProjectPage::clickedCancelButton() {
    // Clean up any connection records created in this wizard session
    if (integrationsPanel)
        integrationsPanel->removeCreatedIntegrations();
    if (aiConnectionsPanel)
        aiConnectionsPanel->removeCreatedAIConnections();

    // Do NOT reset all panels or touch the stacked widget here.
    // The next time this page is shown, showEvent() will call resetAllPanels()
    // and reset the stepper / current index in a clean, consistent state.

    if (source == Source::HomePage)
        emit requestBackPage(0);
    else if (source == Source::ProjectsViewPage)
        emit requestBackPage(1);

    // Just reset the logical flag so a new project can be created next time.
    m_projectCreated = false;
}

void CreateProjectPage::goBackPanel() {
    if (currentIndex > 0) currentIndex--;

    mainPanelContainer->setCurrentIndex(currentIndex);
    scrollArea->verticalScrollBar()->setValue(0);
    stepProgressBar->goBackStep();
}

void CreateProjectPage::goNextPanel()
{
    // If we’re leaving the Database Configuration panel,
    // create the project now so directories (including connectionDirectory)
    // are available before the Integrations / AI Connections panels.
    const int dbIndex = mainPanelContainer->indexOf(databaseConfigPanel);
    if (currentIndex == dbIndex) {
        // Only advance if project creation succeeded
        if (!ensureProjectCreated()) {
            // createProject() already showed an error dialog
            return;
        }
    }

    // If we're on the last panel, treat this as Finish
    if (currentIndex == mainPanelContainer->count() - 1) {
        clickedFinishButton();
        return;
    }

    if (currentIndex < mainPanelContainer->count() - 1)
        currentIndex++;

    mainPanelContainer->setCurrentIndex(currentIndex);
    scrollArea->verticalScrollBar()->setValue(0);
    stepProgressBar->goNextStep();
}

void CreateProjectPage::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    resetAllPanels();
    currentIndex = 0;
    mainPanelContainer->setCurrentIndex(currentIndex);
    stepProgressBar->reset();
}

void CreateProjectPage::resetAllPanels() {
    basicInfoPanel->reset();
    additionalFieldsPanel->reset();
    directoriesPanel->reset();
    databaseConfigPanel->reset();
    integrationsPanel->reset();
    aiConnectionsPanel->reset();
    m_projectCreated = false;  // Allow creating a new project next time
}

void CreateProjectPage::clickedFinishButton()
{
    // Run final validation across all panels
    if (!validateInputs())
        return;

    // Ensure the project exists (if it wasn’t already created when leaving DB config)
    if (!ensureProjectCreated())
        return;

    // Get project info from Data dictionary (set by createProject)
    QString projectPath = basicInfoPanel->getProjectFilePath();
    QString projectName = basicInfoPanel->getProjectName();

    resetAllPanels();
    currentIndex = 0;
    mainPanelContainer->setCurrentIndex(currentIndex);
    stepProgressBar->reset();

    emit projectCreated(projectPath, projectName);
    emit requestBackPage(0);
}

bool CreateProjectPage::validateInputs() {
    QString projectName = basicInfoPanel->getProjectName();
    
    if (projectName.trimmed().isEmpty()) {
        ValidationDialog* dialog = new ValidationDialog(
            "Validation Error", 
            "Please enter a project name.", 
            this);
        dialog->exec();
        dialog->deleteLater();
        return false;
    }
    
    QStringList recordTypes = basicInfoPanel->getSelectedRecordTypes();
    if (recordTypes.isEmpty()) {
        ValidationDialog* dialog = new ValidationDialog(
            "Validation Error", 
            "Please select at least one record type.", 
            this);
        dialog->exec();
        dialog->deleteLater();
        return false;
    }
    
    QString baseDirectory = directoriesPanel->getBaseDirectory();
    if (baseDirectory.trimmed().isEmpty()) {
        ValidationDialog* dialog = new ValidationDialog(
            "Validation Error", 
            "Please specify a base directory.", 
            this);
        dialog->exec();
        dialog->deleteLater();
        return false;
    }
    
    return true;
}

void CreateProjectPage::updateRecordTypes(const QStringList& selectedRecordTypes) {
    // Get all current record type keys from the dictionary
    std::vector<std::string> allRecordTypeKeys = Data::getDictionaryKeysByType("recordType");
    
    // Create a map of record type names to their keys
    std::map<QString, std::string> recordTypeMap;
    for (const auto& key : allRecordTypeKeys) {
        if (key != "DIR" && key != "fileName") {
            QString recordTypeName = QString::fromStdString(Data::getDataValue(key, "recordType"));
            recordTypeMap[recordTypeName] = key;
        }
    }
    
    // Remove record types that were not selected
    for (const auto& pair : recordTypeMap) {
        if (!selectedRecordTypes.contains(pair.first)) {
            Data::removeData(pair.second, "recordType");
        }
    }

    allRecordTypeKeys = Data::getDictionaryKeysByType("recordType");
    allRecordTypeKeys.clear();
}

bool CreateProjectPage::createProject() {
    QString projectName = basicInfoPanel->getProjectName();
    QString description = basicInfoPanel->getDescription();
    QString selectedPath = QDir::cleanPath(basicInfoPanel->getProjectFilePath());

    // baseDirectory = folder that CONTAINS CREAiT
    QString baseDirectory = selectedPath;
    if (QFileInfo(selectedPath).fileName().compare("CREAiT", Qt::CaseInsensitive) == 0) {
        QDir d(selectedPath);
        d.cdUp();
        baseDirectory = d.absolutePath();
    }

    QString type = databaseConfigPanel->getSelectedTool();
    QString credentialFileName = basicInfoPanel->getCredentialFileName();
    QString recordPrefix = basicInfoPanel->getRecordPrefix();
    QStringList recordTypes = basicInfoPanel->getSelectedRecordTypes();
    
    // Set basic project data
    Data::setDataValue("projectName", "project", projectName.toStdString());
    Data::setDataValue("description", "project", description.toStdString());
    Data::setDataValue("type", "project", type.toStdString());
    
    if (!credentialFileName.isEmpty()) {
        Data::setDataValue("credentialsFileName", "project", credentialFileName.toStdString());
    }
    
    if (!recordPrefix.isEmpty()) {
        Data::setDataValue("IDPrefix", "project", recordPrefix.toStdString());
    }
    
    // Update record types based on user selection
    updateRecordTypes(recordTypes);
    
    // Call the backend to create the project
    bool success = projectInfoRecord::createNew(
        baseDirectory.toStdString(),
        projectName.toStdString(),
        type.toLower().toStdString()
    );
    
    if (success) {
        ValidationDialog* dialog = new ValidationDialog(
            "Success", 
            QString("Project '%1' created successfully!").arg(projectName), 
            this);
        dialog->exec();
        dialog->deleteLater();
        qDebug() << "Project created:" << projectName;
        return true;
    } else {
        ValidationDialog* dialog = new ValidationDialog(
            "Error", 
            "Failed to create project. Please check your inputs and try again.", 
            this);
        dialog->exec();
        dialog->deleteLater();
        return false;
    }
}

bool CreateProjectPage::ensureProjectCreated()
{
    // If we already created the project successfully, nothing to do
    if (m_projectCreated)
        return true;

    // createProject() already shows success / error dialogs
    if (createProject()) {
        m_projectCreated = true;
        return true;
    }

    // Project creation failed; let the user correct inputs and try again
    return false;
}

