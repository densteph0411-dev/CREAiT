#include "databaseconfigurationpanel.h"
#include "records.h"       // for Data::setDataValue
#include <QMessageBox>     // for validation warning
#include <QShowEvent>      // for showEvent override

DatabaseConfigurationPanel::DatabaseConfigurationPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    initializeWidgets();

    connect(backButton, &QPushButton::clicked, this, &DatabaseConfigurationPanel::clickedBackButton);
    connect(nextButton, &QPushButton::clicked, this, &DatabaseConfigurationPanel::clickedNextButton);
}

DatabaseConfigurationPanel::~DatabaseConfigurationPanel()
{

}

void DatabaseConfigurationPanel::initializeWidgets() {
    toolNameSelect->addItems({
        "Local",
        "MySQL",
        "SQL",
        "SVN",
        "GIT"
    });
}

void DatabaseConfigurationPanel::clickedBackButton() {
    emit requestBack();
}

void DatabaseConfigurationPanel::clickedNextButton()
{
    // --- Reset styles on all relevant widgets first (like BasicInfoPanel) ---
    auto clearStyle = [](QWidget* w) {
        if (w) {
            w->setStyleSheet(QString());
        }
    };

    clearStyle(cmUrlEdit);
    clearStyle(usernameEdit_2);
    clearStyle(passwordEdit);

    const QString errorStyle = "background-color: rgb(255, 220, 220);";
    bool hasError = false;

    // --- Gather values from the widgets ---
    const QString toolName  = toolNameSelect->currentText().trimmed();
    const QString cmUrl     = cmUrlEdit->text().trimmed();
    const QString userName  = usernameEdit_2->text().trimmed();
    const QString passWord  = passwordEdit->text().trimmed();

    const bool isLocal = (toolName.compare("Local", Qt::CaseInsensitive) == 0);

    // --- Validate only when the tool is NOT "Local" ---
    if (!isLocal) {
        if (cmUrl.isEmpty()) {
            cmUrlEdit->setStyleSheet(errorStyle);
            hasError = true;
        }
        if (userName.isEmpty()) {
            usernameEdit_2->setStyleSheet(errorStyle);
            hasError = true;
        }
        if (passWord.isEmpty()) {
            passwordEdit->setStyleSheet(errorStyle);
            hasError = true;
        }

        if (hasError) {
            QMessageBox::warning(
                this,
                tr("Missing Information"),
                tr("Please fill in all required fields highlighted in pink.")
                );
            return; // stay on this page
        }
    }

    // --- Store values in the Data dictionary ---
    // project-level database/configuration info
    Data::setDataValue("type", "project", toolName.toStdString());
    Data::setDataValue("CMURL",    "project", cmUrl.toStdString());

    // credentials for the selected tool (using existing 'credentials' context)
    Data::setDataValue("userID",   "credentials", userName.toStdString());
    Data::setDataValue("passWord", "credentials", passWord.toStdString());

    // All good: proceed to the next wizard page
    emit requestNext();
}

void DatabaseConfigurationPanel::reset() {
    toolNameSelect->setCurrentIndex(0);
}

QString DatabaseConfigurationPanel::getSelectedTool() const {
    return toolNameSelect->currentText();
}

void DatabaseConfigurationPanel::showEvent(QShowEvent *event)
{
    // Always call base implementation first
    QWidget::showEvent(event);

    // --- Load shared CREAiT credentials from the Data store ---
    const QString storedUserId =
        QString::fromStdString(Data::getDataValue("userID", "credentials"));
    if (!storedUserId.isEmpty()) {
        usernameEdit_2->setText(storedUserId);
    }

    const QString storedPassword =
        QString::fromStdString(Data::getDataValue("passWord", "credentials"));
    if (!storedPassword.isEmpty()) {
        passwordEdit->setText(storedPassword);
    }
}

