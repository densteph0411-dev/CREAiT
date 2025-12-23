#include "basicsettingspage.h"
#include <QGridLayout>

BasicSettingsPage::BasicSettingsPage(QWidget *parent)
    : QWidget(parent)
{
    initializeWidgets();
}

BasicSettingsPage::~BasicSettingsPage()
{

}

void BasicSettingsPage::initializeWidgets() {
    setupUi(this);

    container->adjustSize();

    makeBasicInfoAccordion();
    makeDirectoriesAccordion();
    makeDatabaseAccordion();
    makeAdditionalFieldsAccordion();

    connect(editButton, &QPushButton::clicked, this, &BasicSettingsPage::clickedEditButton);
}

void BasicSettingsPage::makeBasicInfoAccordion() {
    basicInfoAccordion->setTitle("Basic Info");
    basicInfoAccordion->contentArea->setFixedHeight(300);

    QGridLayout* layout = new QGridLayout(basicInfoAccordion->contentArea);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(24);

    projectNameCell = new AutomationCellProperty();
    projectNameCell->icon->setIcon(QIcon(":images/ProjectSettings/project_name.svg"));
    projectNameCell->titleLabel->setText("Project Name");
    layout->addWidget(projectNameCell, 0, 0);

    typeCell = new AutomationCellProperty();
    typeCell->icon->setIcon(QIcon(":images/ProjectSettings/type.svg"));
    typeCell->titleLabel->setText("Type");
    layout->addWidget(typeCell, 0, 1);

    idPrefixCell = new AutomationCellProperty();
    idPrefixCell->icon->setIcon(QIcon(":images/ProjectSettings/id_prefix.svg"));
    idPrefixCell->titleLabel->setText("ID Prefix");
    layout->addWidget(idPrefixCell, 1, 0);

    credentialFileNameCell = new AutomationCellProperty();
    credentialFileNameCell->icon->setIcon(QIcon(":images/ProjectSettings/credential_file_name.svg"));
    credentialFileNameCell->titleLabel->setText("Credential File Name");
    layout->addWidget(credentialFileNameCell, 1, 1);

    descriptionCell = new AutomationCellProperty();
    descriptionCell->icon->setIcon(QIcon(":images/ProjectSettings/description.svg"));
    descriptionCell->titleLabel->setText("Description");
    layout->addWidget(descriptionCell, 2, 0);
}

void BasicSettingsPage::makeDirectoriesAccordion() {
    directoriesAccordion->setTitle("Directories");
    directoriesAccordion->contentArea->setFixedHeight(300);

    QGridLayout* layout = new QGridLayout(directoriesAccordion->contentArea);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(24);

    baseDirectoryCell = new AutomationCellProperty();
    baseDirectoryCell->icon->setIcon(QIcon(":images/ProjectSettings/directory.svg"));
    baseDirectoryCell->titleLabel->setText("Base Directory");
    layout->addWidget(baseDirectoryCell, 0, 0);

    programDirectoryCell = new AutomationCellProperty();
    programDirectoryCell->icon->setIcon(QIcon(":images/ProjectSettings/directory.svg"));
    programDirectoryCell->titleLabel->setText("Program Directory");
    layout->addWidget(programDirectoryCell, 0, 1);

    projectDirectoryCell = new AutomationCellProperty();
    projectDirectoryCell->icon->setIcon(QIcon(":images/ProjectSettings/directory.svg"));
    projectDirectoryCell->titleLabel->setText("Project Directory");
    layout->addWidget(projectDirectoryCell, 1, 0);

    recordDirectoryCell = new AutomationCellProperty();
    recordDirectoryCell->icon->setIcon(QIcon(":images/ProjectSettings/directory.svg"));
    recordDirectoryCell->titleLabel->setText("Record Directory");
    layout->addWidget(recordDirectoryCell, 1, 1);

    imageDirectoryCell = new AutomationCellProperty();
    imageDirectoryCell->icon->setIcon(QIcon(":images/ProjectSettings/directory.svg"));
    imageDirectoryCell->titleLabel->setText("Image Directory");
    layout->addWidget(imageDirectoryCell, 2, 0);
}

void BasicSettingsPage::makeDatabaseAccordion() {
    databaseAccordion->setTitle("Databases");
    databaseAccordion->contentArea->setFixedHeight(208);

    QGridLayout* layout = new QGridLayout(databaseAccordion->contentArea);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(24);

    databaseNameCell = new AutomationCellProperty();
    databaseNameCell->icon->setIcon(QIcon(":images/ProjectSettings/database_name.svg"));
    databaseNameCell->titleLabel->setText("Database Name");
    layout->addWidget(databaseNameCell, 0, 0);

    databaseUrlCell = new AutomationCellProperty();
    databaseUrlCell->icon->setIcon(QIcon(":images/ProjectSettings/database_url.svg"));
    databaseUrlCell->titleLabel->setText("Database URL");
    layout->addWidget(databaseUrlCell, 0, 1);

    cmUrlCell = new AutomationCellProperty();
    cmUrlCell->icon->setIcon(QIcon(":images/ProjectSettings/cm_url.svg"));
    cmUrlCell->titleLabel->setText("CM URL");
    layout->addWidget(cmUrlCell, 1, 0);

    toolNameCell = new AutomationCellProperty();
    toolNameCell->icon->setIcon(QIcon(":images/ProjectSettings/tool_name.svg"));
    toolNameCell->titleLabel->setText("Tool Name");
    layout->addWidget(toolNameCell, 1, 1);
}

void BasicSettingsPage::makeAdditionalFieldsAccordion() {
    additionalFieldsAccordion->setTitle("Additional Fields");
    additionalFieldsAccordion->contentArea->setFixedHeight(108);

    QGridLayout* layout = new QGridLayout(additionalFieldsAccordion->contentArea);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(24);

    field1Cell = new AutomationCellProperty();
    field1Cell->icon->setIcon(QIcon(":images/ProjectSettings/additional_field.svg"));
    field1Cell->description->hide();
    layout->addWidget(field1Cell, 0, 0);

    field2Cell = new AutomationCellProperty();
    field2Cell->icon->setIcon(QIcon(":images/ProjectSettings/additional_field.svg"));
    field2Cell->description->hide();
    layout->addWidget(field2Cell, 0, 1);
}

void BasicSettingsPage::loadWidgetsSettings() {
    const BasicInfoData& basic = ProjectSettingsController::getBasicInfo();
    projectNameCell->description->setText(basic.projectName);
    typeCell->description->setText(basic.type);
    idPrefixCell->description->setText(basic.IDPrefix);
    credentialFileNameCell->description->setText(basic.credentialsFileName);
    descriptionCell->description->setText(basic.description);

    const DirectoriesData& dirs = ProjectSettingsController::getDirectories();
    baseDirectoryCell->description->setText(dirs.baseDirectory);
    programDirectoryCell->description->setText(dirs.programDirectory);
    projectDirectoryCell->description->setText(dirs.projectDirectory);
    recordDirectoryCell->description->setText(dirs.recordDirectory);
    imageDirectoryCell->description->setText(dirs.imageDirectory);

    const DatabasesData& db = ProjectSettingsController::getDatabases();
    databaseNameCell->description->setText(db.databaseName);
    databaseUrlCell->description->setText(db.databaseUrl);
    cmUrlCell->description->setText(db.CMUrl);
    toolNameCell->description->setText(db.toolName);

    const QStringList& fields = ProjectSettingsController::getAdditionalFields();
    field1Cell->titleLabel->setText(fields.value(0, "—"));
    field2Cell->titleLabel->setText(fields.value(1, "—"));
}


void BasicSettingsPage::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    loadWidgetsSettings();
}

void BasicSettingsPage::clickedEditButton() {
    emit requestEdit(4);
}
