#include "standardcreatedialog.h"
#include <reviewscontroller.h>
#include <QPainter>
#include <QFileInfo>

StandardCreateDialog::StandardCreateDialog(QWidget *parent)
    : QDialog(parent), index(-1), reviewsPath("reviews")
{
    initializeWidgets();
}

StandardCreateDialog::~StandardCreateDialog()
{

}

void StandardCreateDialog::initializeWidgets() {
    setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    uploadFileWidget->uploadTipLabel->setText("Upload Document here");

    connect(uploadFileWidget, &DropFileWidget::requestSelectFile, this, &StandardCreateDialog::handleFileSelect);

    connect(closeButton, &QPushButton::clicked, this, &StandardCreateDialog::clickedCloseButton);
    connect(cancelButton, &QPushButton::clicked, this, &StandardCreateDialog::clickedCancelButton);
    connect(saveButton, &QPushButton::clicked, this, &StandardCreateDialog::clickedUploadButton);
}

void StandardCreateDialog::setData(int id) {
    index = id;

    if (index == -1) {
        titleEdit->setText("");
        uploadFileWidget->clear();
    } else {
        StandardData data = ReviewsController::getStandardById(index);
        titleEdit->setText(data.title);
        handleFileSelect(data.filePath);
    }
}

void StandardCreateDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void StandardCreateDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}

void StandardCreateDialog::handleFileSelect(const QString& filePath) {
    const QFileInfo fileInfo(filePath);
    const QString filename = fileInfo.fileName();
    const qint64 size = fileInfo.size();
    uploadFileWidget->fileNameLabel->setText(filename);
    uploadFileWidget->fileSizeLabel->setText(uploadFileWidget->formatFileSize(size));

    originFielPath = filePath;
}

void StandardCreateDialog::clickedCloseButton() {
    reject();
}

void StandardCreateDialog::clickedCancelButton() {
    reject();
}

void StandardCreateDialog::clickedUploadButton() {
    if (originFielPath.isEmpty() || !QFile::exists(originFielPath)) {
        qDebug() << "Invalid file to upload";
        return;
    }

    QFileInfo fileInfo(originFielPath);
    QString destPath = reviewsPath + "/" + fileInfo.fileName();

    if (!QFile::exists(destPath))
        QFile::copy(originFielPath, destPath);

    StandardData data;
    data.title = titleEdit->text();
    data.filePath = destPath;

    if (index == -1)
        ReviewsController::addStandard(data);
    else
        ReviewsController::updateStandard(index, data);

    accept();
}
