#include "integrationaddeditdialog.h"
#include <QPainter>
#include <projectsettingscontroller.h>

IntegrationAddEditDialog::IntegrationAddEditDialog(int type, QWidget *parent)
    : QDialog(parent), type(type), integrationId(0)
{
    setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    dialogTitle->setText(type == 0 ? "Add Integration" : "Edit Integration");
    toolSelect->addItems({"JAMA", "Polarion", "DOORs"});

    connect(closeButton, &QPushButton::clicked, this, &IntegrationAddEditDialog::clickedCloseButton);
    connect(cancelButton, &QPushButton::clicked, this, &IntegrationAddEditDialog::clickedCancelButton);
    connect(saveButton, &QPushButton::clicked, this, &IntegrationAddEditDialog::clickedSaveButton);
}

IntegrationAddEditDialog::~IntegrationAddEditDialog()
{

}

void IntegrationAddEditDialog::setData(int id) {

    IntegrationData data = ProjectSettingsController::getIntegrationById(id);
    toolSelect->setCurrentIndex(data.toolDisplayIndex);
    urlEdit->setText(data.url);
    apiKeyEdit->setText(data.Key);
    usernameEdit->setText(data.username);
    passwordEdit->setText(data.password);
}

void IntegrationAddEditDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void IntegrationAddEditDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}

void IntegrationAddEditDialog::clickedCancelButton() {
    emit clickedCloseButton();
}

void IntegrationAddEditDialog::clickedCloseButton() {
    reject();
}

void IntegrationAddEditDialog::clickedSaveButton() {
    IntegrationData data;
    data.toolDisplayIndex = toolSelect->currentIndex();
    data.url = urlEdit->text();
    data.Key = apiKeyEdit->text();
    data.username = usernameEdit->text();
    data.password = passwordEdit->text();

    if (type == 0) {
        ProjectSettingsController::addIntegration(data);
        emit requestRefreshAll();
    } else {
        ProjectSettingsController::updateIntegration(0, data);
        emit requestRefresh(0, integrationId);
    }


    accept();
}
