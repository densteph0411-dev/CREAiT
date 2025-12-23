#include "basicsettingseditpage.h"


BasicSettingsEditPage::BasicSettingsEditPage(QWidget *parent)
    : QWidget(parent), fieldNextId(0)
{
    initializeWidgets();
}

BasicSettingsEditPage::~BasicSettingsEditPage() {

}

void BasicSettingsEditPage::initializeWidgets() {
    setupUi(this);

    makeBasicInfoAccordion();
    makeDirectoriesAccordion();
    makeDatabaseAccordion();
    makeAdditionalFieldsAccordion();

    emit clickedAddFieldButton();

    connect(backButton, &QPushButton::clicked, this, &BasicSettingsEditPage::clickedBackButton);
    connect(cancelButton, &QPushButton::clicked, this, &BasicSettingsEditPage::clickedCancelButton);
    connect(saveButton, &QPushButton::clicked, this, &BasicSettingsEditPage::clickedSaveButton);
}

void BasicSettingsEditPage::makeBasicInfoAccordion() {
    basicInfoAccordion->setTitle("Basic Info");
    basicInfoAccordion->contentArea->setFixedHeight(332);

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setContentsMargins(24, 24, 24, 24);
    gridLayout->setSpacing(24);

    projectNameEdit = new CustomLineEdit();
    typeEdit  = new CustomLineEdit();
    idPrefixEdit = new CustomLineEdit();
    credentialsFileNameEdit = new CustomLineEdit();
    descriptionEdit = new CustomTextEdit();

    projectNameEdit->label->setText("Project Name");
    typeEdit->label->setText("Type");
    idPrefixEdit->label->setText("ID  Prefix");
    credentialsFileNameEdit->label->setText("Credential File Name");
    descriptionEdit->label->setText("Description");

    gridLayout->addWidget(projectNameEdit, 0, 0);
    gridLayout->addWidget(typeEdit, 0, 1);
    gridLayout->addWidget(idPrefixEdit, 1, 0);
    gridLayout->addWidget(credentialsFileNameEdit, 1, 1);
    gridLayout->addWidget(descriptionEdit, 2, 0, 1, 2);

    basicInfoAccordion->contentArea->setLayout(gridLayout);
}

void BasicSettingsEditPage::makeDirectoriesAccordion() {
    directoriesAccordion->setTitle("Directories");
    directoriesAccordion->contentArea->setFixedHeight(300);

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setContentsMargins(24, 24, 24, 24);
    gridLayout->setSpacing(24);

    baseDirEdit = new DirSelectEdit();
    programDirEdit = new DirSelectEdit();
    projectDirEdit = new DirSelectEdit();
    recordDirEdit = new DirSelectEdit();
    imageDirEdit = new DirSelectEdit();

    baseDirEdit->label->setText("Base Directory");
    programDirEdit->label->setText("Program Directory");
    projectDirEdit->label->setText("Project Directory");
    recordDirEdit->label->setText("Record Directory");
    imageDirEdit->label->setText("Image Directory");

    gridLayout->addWidget(baseDirEdit, 0, 0);
    gridLayout->addWidget(programDirEdit, 0, 1);
    gridLayout->addWidget(projectDirEdit, 1, 0);
    gridLayout->addWidget(recordDirEdit, 1, 1);
    gridLayout->addWidget(imageDirEdit, 2, 0);

    directoriesAccordion->contentArea->setLayout(gridLayout);
}

void BasicSettingsEditPage::makeDatabaseAccordion() {
    databaseAccordion->setTitle("Databases");
    databaseAccordion->contentArea->setFixedHeight(208);

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setContentsMargins(24, 24, 24, 24);
    gridLayout->setSpacing(24);

    databaseNameEdit = new CustomLineEdit();
    databaseUrlEdit = new CustomLineEdit();
    cmUrlEdit = new CustomLineEdit();
    toolNameEdit = new CustomLineEdit();

    databaseNameEdit->label->setText("Database Name");
    databaseUrlEdit->label->setText("Database URL");
    cmUrlEdit->label->setText("CM URL");
    toolNameEdit->label->setText("Tool Name");

    gridLayout->addWidget(databaseNameEdit, 0, 0);
    gridLayout->addWidget(databaseUrlEdit, 0, 1);
    gridLayout->addWidget(cmUrlEdit, 1, 0);
    gridLayout->addWidget(toolNameEdit, 1, 1);

    databaseAccordion->contentArea->setLayout(gridLayout);
}

void BasicSettingsEditPage::makeAdditionalFieldsAccordion() {
    additionalFieldsAccordion->setTitle("Additional Fields");
    additionalFieldsAccordion->contentArea->setFixedHeight(24 + 10 + 25 + 10 + 0 + 10 + 40 + 24);

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setContentsMargins(24, 24, 24, 24);
    gridLayout->setSpacing(10);
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);

    QLabel* additionalFiedlLabel = new QLabel();
    additionalFiedlLabel->setText("Field Name");
    gridLayout->addWidget(additionalFiedlLabel, 0, 0);

    fieldTextContainer = new QWidget();
    fieldTextContainer->setFixedHeight(10);
    fieldTextContainerLayout = new QVBoxLayout(fieldTextContainer);
    fieldTextContainerLayout->setContentsMargins(0, 0, 0, 0);
    fieldTextContainerLayout->setSpacing(10);

    // AdditionalFieldText* text = new AdditionalFieldText(0);

    gridLayout->addWidget(fieldTextContainer, 1, 0);

    addFieldTextButton = new QPushButton("Add Additionl Field");
    addFieldTextButton->setObjectName("additionalFieldAddButton");
    addFieldTextButton->setFixedWidth(300);
    addFieldTextButton->setFixedHeight(40);

    connect(addFieldTextButton, &QPushButton::clicked, this, &BasicSettingsEditPage::clickedAddFieldButton);

    gridLayout->addWidget(addFieldTextButton, 2, 0);

    additionalFieldsAccordion->contentArea->setLayout(gridLayout);
}

void BasicSettingsEditPage::loadWidgetsSettings() {
    // --- Basic Info ---
    BasicInfoData basic = ProjectSettingsController::getBasicInfo();
    projectNameEdit->lineEdit->setText(basic.projectName);
    typeEdit->lineEdit->setText(basic.type);
    idPrefixEdit->lineEdit->setText(basic.IDPrefix);
    credentialsFileNameEdit->lineEdit->setText(basic.credentialsFileName);
    descriptionEdit->textEdit->setPlainText(basic.description);

    // --- Directories ---
    DirectoriesData dirs = ProjectSettingsController::getDirectories();
    baseDirEdit->lineEdit->setText(dirs.baseDirectory);
    programDirEdit->lineEdit->setText(dirs.programDirectory);
    projectDirEdit->lineEdit->setText(dirs.projectDirectory);  // typo fixed here
    recordDirEdit->lineEdit->setText(dirs.recordDirectory);
    imageDirEdit->lineEdit->setText(dirs.imageDirectory);

    // --- Database ---
    DatabasesData db = ProjectSettingsController::getDatabases();
    databaseNameEdit->lineEdit->setText(db.databaseName);
    databaseUrlEdit->lineEdit->setText(db.databaseUrl);
    cmUrlEdit->lineEdit->setText(db.CMUrl);
    toolNameEdit->lineEdit->setText(db.toolName);
}

void BasicSettingsEditPage::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    loadWidgetsSettings();
}

void BasicSettingsEditPage::clickedBackButton() {
    emit requestBackPage(0);
}

void BasicSettingsEditPage::clickedCancelButton() {
    emit requestBackPage(0);
}

void BasicSettingsEditPage::clickedSaveButton() {
    BasicInfoData basicInfoData;
    basicInfoData.projectName = projectNameEdit->lineEdit->text();
    basicInfoData.type = typeEdit->lineEdit->text();
    basicInfoData.IDPrefix = idPrefixEdit->lineEdit->text();
    basicInfoData.credentialsFileName = credentialsFileNameEdit->lineEdit->text();
    basicInfoData.description = descriptionEdit->textEdit->toPlainText();
    ProjectSettingsController::setBasicInfo(basicInfoData);

    DirectoriesData directoriesData;
    directoriesData.baseDirectory = baseDirEdit->lineEdit->text();
    directoriesData.programDirectory = programDirEdit->lineEdit->text();
    directoriesData.projectDirectory = projectDirEdit->lineEdit->text();
    directoriesData.recordDirectory = recordDirEdit->lineEdit->text();
    directoriesData.imageDirectory = imageDirEdit->lineEdit->text();
    ProjectSettingsController::setDirectories(directoriesData);

    DatabasesData databasesData;
    databasesData.databaseName = databaseNameEdit->lineEdit->text();
    databasesData.databaseUrl = databaseUrlEdit->lineEdit->text();
    databasesData.CMUrl = cmUrlEdit->lineEdit->text();
    databasesData.toolName = toolNameEdit->lineEdit->text();
    ProjectSettingsController::setDatabases(databasesData);

    ProjectSettingsController::saveProjectBasicInfo();

    clickedBackButton();
}

void BasicSettingsEditPage::clickedAddFieldButton() {
    int id = fieldNextId++;
    AdditionalFieldText* text = new AdditionalFieldText(id);
    fieldTexts.insert(id, text);
    fieldTextContainerLayout->addWidget(text);
    fieldTextContainer->setFixedHeight(fieldTextContainer->height() + 40 + 10);
    additionalFieldsAccordion->contentArea->setFixedHeight(additionalFieldsAccordion->contentArea->height() + 40 + 10);

    connect(text, &AdditionalFieldText::requestDelete, this, &BasicSettingsEditPage::handleDeleteField);
}

void BasicSettingsEditPage::handleDeleteField(int id) {
    if (fieldTexts.size() <= 1) return;

    if (fieldTexts.contains(id)) {
        auto fieldText = fieldTexts.take(id);
        fieldTextContainerLayout->removeWidget(fieldText);
        fieldText->deleteLater();
        fieldTextContainer->setFixedHeight(fieldTextContainer->height() - 40 - 10);
        additionalFieldsAccordion->contentArea->setFixedHeight(additionalFieldsAccordion->contentArea->height() - 40 -10);
    }
}
