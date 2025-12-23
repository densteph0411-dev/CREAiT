#include "dropfilewidget.h"
#include <QMouseEvent>
#include <QFileInfo>

DropFileWidget::DropFileWidget(QWidget *parent)
    : QWidget(parent)
{
    initializeWidgets();   
}

DropFileWidget::~DropFileWidget() {

}

void DropFileWidget::initializeWidgets() {
    setupUi(this);

    setAcceptDrops(true);
    setCursor(Qt::PointingHandCursor);

    fileNameLabel->setText("");
    fileSizeLabel->setText("");
    progressBar->setValue(0);

    connect(fileSelectButton, &QToolButton::clicked, this, &DropFileWidget::clickedFileSelectButton);
    connect(closeButton, &QPushButton::clicked, this, &DropFileWidget::clickedCloseButton);
}

void DropFileWidget::clear() {
    fileNameLabel->setText("");
    fileSizeLabel->setText("");
    progressBar->setValue(0);
}

void DropFileWidget::handleFile(const QString& filePath) {
    QFileInfo fileInfo(filePath);

    fileNameLabel->setText(fileInfo.fileName());
    fileSizeLabel->setText(formatFileSize(fileInfo.size()));
    progressBar->setValue(0);

    emit requestSelectFile(filePath);
}

QString DropFileWidget::formatFileSize(qint64 bytes) const {
    const QStringList units = {"B", "kB", "MB", "GB", "TB"};
    double size = bytes;
    int unitIndex = 0;

    while (size >= 1024.0 && unitIndex < units.size() - 1) {
        size /= 1024.0;
        ++unitIndex;
    }

    return QString::number(size, 'f', 1) + " " + units[unitIndex];
}

void DropFileWidget::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void DropFileWidget::dropEvent(QDropEvent* event) {
    QList<QUrl> urls = event->mimeData()->urls();
    if (!urls.isEmpty()) {
        QString filePath = urls.first().toLocalFile();
        handleFile(filePath);
    }
}

void DropFileWidget::mousePressEvent(QMouseEvent* event) {
    Q_UNUSED(event);

    if (event->button() == Qt::LeftButton) {
        QString filePath = QFileDialog::getOpenFileName(this, "Select a file");
        if (!filePath.isEmpty()) {
            handleFile(filePath);
        }
    }
}

void DropFileWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    Q_UNUSED(event);

    if (event->button() == Qt::LeftButton) {
        QString filePath = QFileDialog::getOpenFileName(this, "Select a file");
        if (!filePath.isEmpty()) {
            handleFile(filePath);
        }
    }
}

void DropFileWidget::clickedFileSelectButton() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select a file");
    if (!filePath.isEmpty()) {
        handleFile(filePath);
    }
}

void DropFileWidget::clickedCloseButton() {
    clear();
}
