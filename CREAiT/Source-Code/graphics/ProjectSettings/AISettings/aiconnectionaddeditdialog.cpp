#include "aiconnectionaddeditdialog.h"
#include <QPainter>

AIConnectionAddEditDialog::AIConnectionAddEditDialog(int type, QWidget *parent)
    : QDialog(parent), type(type)
{
    setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    aiSelect->addItems({ "Gemini", "OpenAI", "GreekSearch" });
    versionSelect->addItems({ "1.0.1", "2.4.0", "3.2.1" });

    connect(closeButton, &QPushButton::clicked, this, &AIConnectionAddEditDialog::clickedCloseButton);
    connect(cancelButton, &QPushButton::clicked, this, &AIConnectionAddEditDialog::clickedCancelButton);
    connect(saveButton, &QPushButton::clicked, this, &AIConnectionAddEditDialog::clickedSaveButton);
}

AIConnectionAddEditDialog::~AIConnectionAddEditDialog()
{

}

void AIConnectionAddEditDialog::setData(int id) {
    AIConnectionData data = ProjectSettingsController::getAIConnectionById(id);

    urlEdit->setText(data.url);
    aiSelect->setCurrentText(data.aiModel);
    versionSelect->setCurrentText(data.version);
    apiKeyEdit->setText(data.Key);
}

void AIConnectionAddEditDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void AIConnectionAddEditDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}

void AIConnectionAddEditDialog::clickedCancelButton() {
    emit clickedCloseButton();
}

void AIConnectionAddEditDialog::clickedCloseButton() {
    reject();
}

void AIConnectionAddEditDialog::clickedSaveButton() {
    AIConnectionData data;
    data.url = urlEdit->text();
    data.aiModel = aiSelect->currentText();
    data.version = versionSelect->currentText();
    data.Key = apiKeyEdit->text();

    if (type == 0) {
        ProjectSettingsController::addAIConnection(data);
        emit requestRefreshAll();
    } else {
        ProjectSettingsController::updateAIConnection(aiConnectionId, data);
        emit requestRefresh(0, aiConnectionId);
    }
    accept();
}
