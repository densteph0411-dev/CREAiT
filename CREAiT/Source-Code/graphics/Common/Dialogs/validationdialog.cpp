#include "validationdialog.h"
#include "ui_validationdialog.h"
#include <QPainter>
#include <QApplication>

ValidationDialog::ValidationDialog(const QString& title, const QString& message, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    dialogTitle->setText(title);
    confirmTitleLabel->setText(title);
    messageLabel->setText(message);
    
    // Set icon based on title
    QString iconPath;
    if (title.contains("Success", Qt::CaseInsensitive)) {
        iconPath = "background-image: url(:/images/Project/success-icon.svg);";
    } else if (title.contains("Error", Qt::CaseInsensitive)) {
        iconPath = "background-image: url(:/images/Project/error-icon.svg);";
    } else {
        iconPath = "background-image: url(:/images/Project/warning-icon.svg);";
    }
    iconWidget->setStyleSheet(iconPath);
}

ValidationDialog::~ValidationDialog()
{

}

void ValidationDialog::setMessage(const QString& message)
{
    messageLabel->setText(message);
}

void ValidationDialog::onOkButtonClicked()
{
    accept();
}

void ValidationDialog::onCloseButtonClicked()
{
    reject();
}

void ValidationDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void ValidationDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    if (mainWindow) {
        QRect parentRect = mainWindow->rect();
        QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
        setFixedSize(parentRect.size());
        move(topLeft);
    }

    QDialog::showEvent(event);
}

