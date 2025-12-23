#include "reviewactiondialog.h"
#include <QPainter>

ReviewActionDialog::ReviewActionDialog(QWidget *parent)
    : QDialog(parent), index(-1), actionType(-1)
{
    initializeWidgets();
}

ReviewActionDialog::~ReviewActionDialog() {

}

void ReviewActionDialog::initializeWidgets() {
    setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    versionSelect->addItems({
        "2.6.8.91",
        "2.6.8.94",
        "2.6.8.96",
        "2.6.8.99",
    });

    connect(closeButton, &QPushButton::clicked, this, &ReviewActionDialog::clickedCloseButton);
    connect(cancelButton, &QPushButton::clicked, this, &ReviewActionDialog::clickedCancelButton);
    connect(saveButton, &QPushButton::clicked, this, &ReviewActionDialog::clickedSaveButton);
}

void ReviewActionDialog::setData(int id, int type, int versionId, const QString& version, const QString& description) {
    index = id;
    actionType = type;
    VersionContainer->hide();
    descriptionLabel->setText("Comment");
    saveButton->setStyleSheet(R"(
        background: #FBAE17;
    )");

    switch(actionType) {
    case Implement:
        VersionContainer->show();
        dialogTitle->setText("Implement");
        descriptionLabel->setText("Descritpion");
        saveButton->setText("Implement");
        break;
    case Comment:
        dialogTitle->setText("Comment");
        saveButton->setText("Add Comment");
        break;
    case VerifyFinding:
        dialogTitle->setText("Finding");
        saveButton->setText("Verify Finding");
        break;
    case RejectFinding:
        dialogTitle->setText("Finding");
        saveButton->setText("Reject Finding");
        saveButton->setStyleSheet(R"(
            background: #B82132;
        )");
        break;
    }

    bodyContainer->adjustSize();

    dialogContainer->setFixedHeight(actionType == Implement ? 420 : 344);

    if (index == -1) return;

    if (actionType == Implement) {
        if(versionId == -1) versionSelect->setCurrentText(version);
        else versionSelect->setCurrentIndex(versionId);
    }

    descriptionEdit->setText(description);
}

void ReviewActionDialog::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void ReviewActionDialog::showEvent(QShowEvent *event) {
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}

void ReviewActionDialog::clickedCloseButton() {
    reject();
}

void ReviewActionDialog::clickedCancelButton() {
    reject();
}

void ReviewActionDialog::clickedSaveButton() {
    accept();
}
