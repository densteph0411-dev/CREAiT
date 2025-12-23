#include "basicinfopanel.h"
#include "createtypedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QMap>
#include <algorithm>
#include <records.h>

BasicInfoPanel::BasicInfoPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    // Save the original stylesheet for the MultiSelectWidget
    m_recordTypeSelectDefaultStyle = recordTypeSelect->styleSheet();

    connect(nextButton, &QPushButton::clicked,
            this, &BasicInfoPanel::clickedNextButton);
    connect(projectFilePathBrowseButton, &QPushButton::clicked,
            this, &BasicInfoPanel::clickedProjectFilePathBrowseButton);

    // "+" button for creating new record types
    connect(recordTypeAddButton, &QPushButton::clicked,
            this, &BasicInfoPanel::clickedRecordTypeAddButton);

    // Load record types from Data dictionary once
    loadRecordTypesFromData();

    // If a directory already exists, then autopopulate it.
    QString programDirectory =
        QString::fromStdString(Data::getDataValue("programDirectory", "project"));
    projectFilePathEdit->setText(programDirectory);
}

BasicInfoPanel::~BasicInfoPanel() {

}

void BasicInfoPanel::clickedNextButton()
{
    auto clearStyle = [](QWidget* w) {
        if (w) w->setStyleSheet("");
    };

    // Clear styles on the simple widgets
    clearStyle(projectNameEdit);
    clearStyle(descriptionEdit);
    clearStyle(projectFilePathEdit);

    // For the MultiSelectWidget, restore its original stylesheet
    recordTypeSelect->setStyleSheet(m_recordTypeSelectDefaultStyle);

    const QString errorStyle = "background-color: rgb(255, 220, 220);";
    bool hasError = false;

    // --- Read values from the page ---
    const QString projectName      = projectNameEdit->text().trimmed();
    const QString description      = descriptionEdit->toPlainText().trimmed();
    const QString projectFilePath  = projectFilePathEdit->text().trimmed();

    // Selected record types from the MultiSelectWidget
    QStringList recordTypes = recordTypeSelect->selectedItems();

    // --- Validate required fields ---

    if (projectName.isEmpty()) {
        projectNameEdit->setStyleSheet(errorStyle);
        hasError = true;
    }

    if (recordTypes.isEmpty()) {
        // recordTypeSelect is required to have something selected
        // Do NOT overwrite its stylesheet; just mark error and maybe focus it
        hasError = true;
        recordTypeSelect->setFocus();
    }

    if (description.isEmpty()) {
        descriptionEdit->setStyleSheet(errorStyle);
        hasError = true;
    }

    if (projectFilePath.isEmpty()) {
        projectFilePathEdit->setStyleSheet(errorStyle);
        hasError = true;
    }

    // If anything is missing, stay on this page and do NOT emit requestNext()
    if (hasError) {
        // Optional: show a small message dialog (or use your existing validation dialog)
        QMessageBox::warning(
            this,
            tr("Missing Information"),
            tr("Please fill in all required fields highlighted")
            );
        return;  // <-- stays on this page
    }

    // -----------------------------------------------------------------
    // Synchronize record types in the Data dictionary (context "recordType")
    // -----------------------------------------------------------------

    // Build a map of existing recordType entries: display name -> key (ID prefix)
    std::vector<std::string> existingKeys = Data::getDictionaryKeysByType("recordType");
    QMap<QString, std::string> valueToKey;

    // Now that everything is valid and stored, we can safely move to the next page
    emit requestNext();
}

void BasicInfoPanel::reset() {
    projectNameEdit->clear();
    descriptionEdit->clear();

    // If a project is already loaded, pre-populate the value to keep all projects together
    projectFilePathEdit->clear();
    projectFilePathEdit->setText(QString::fromStdString(
        Data::getDataValue("programDirectory","project")));

    // Enter credentials file name
    credentialFileNameEdit->clear();
    credentialFileNameEdit->setText(QString::fromStdString(
        Data::getDataValue("fileNameOnly","credentials")));

    recordPrefixEdit->clear();

    // Only reset selection; items were loaded once in the ctor
    recordTypeSelect->clearSelections();
}

QString BasicInfoPanel::getProjectName() const {
    return projectNameEdit->text();
}

QString BasicInfoPanel::getDescription() const {
    return descriptionEdit->toPlainText();
}

QString BasicInfoPanel::getProjectFilePath() const {
    return projectFilePathEdit->text();
}

QString BasicInfoPanel::getCredentialFileName() const {
    return credentialFileNameEdit->text();
}

QString BasicInfoPanel::getRecordPrefix() const {
    return recordPrefixEdit->text();
}


QStringList BasicInfoPanel::getSelectedRecordTypes() const {
    return recordTypeSelect->selectedItems();
}

void BasicInfoPanel::clickedProjectFilePathBrowseButton() {
    QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("Select Directory"),
        "",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    if (!dir.isEmpty()) {
        projectFilePathEdit->setText(dir);
    }
}

void BasicInfoPanel::clickedRecordTypeAddButton()
{
    // Existing prefixes to show in the dropdown (keys of context "recordType")
    QStringList existingPrefixes;
    std::vector<std::string> keys = Data::getDictionaryKeysByType("recordType");
    for (const auto &key : keys) {
        if (key == "DIR" || key == "fileNameOnly")
            continue;
        existingPrefixes << QString::fromStdString(key);
    }

    CreateTypeDialog dlg(this);
    dlg.setExistingPrefixes(existingPrefixes);

    if (dlg.exec() == QDialog::Accepted) {
        const QString typeName = dlg.typeName().trimmed();
        const QString prefix   = dlg.prefix().trimmed();

        if (typeName.isEmpty() || prefix.isEmpty())
            return;

        // Persist the new record type (key = prefix, value = type name)
        Data::addData(prefix.toStdString(),
                      "recordType",
                      typeName.toStdString(),
                      "true",
                      "The prefix for the record ID");

        // Add to the MultiSelectWidget and select it
        recordTypeSelect->addItem(typeName);
        recordTypeSelect->setCurrentItem(typeName);
    }
}

void BasicInfoPanel::loadRecordTypesFromData()
{
    std::vector<std::string> recordTypeKeys =
        Data::getDictionaryKeysByType("recordType");
    QStringList recordTypeNames;

    for (const auto &key : recordTypeKeys) {
        if (key == "DIR" || key == "fileNameOnly")
            continue;

        const QString recordTypeName =
            QString::fromStdString(Data::getDataValue(key, "recordType"));
        recordTypeNames.append(recordTypeName);
    }

    recordTypeSelect->addItems(recordTypeNames);
    recordTypeSelect->clearSelections();
}



