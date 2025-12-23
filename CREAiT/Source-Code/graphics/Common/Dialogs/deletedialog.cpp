#include "deletedialog.h"
#include "ui_deletedialog.h"
#include <QPainter>

DeleteDialog::DeleteDialog(const QString& title, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    dialogTitle->setText(title);
}

DeleteDialog::~DeleteDialog()
{

}

void DeleteDialog::onCancelButtonClicked()
{
    reject();
}

void DeleteDialog::onCloseButtonClicked()
{
    reject();
}

void DeleteDialog::onDeleteButtonClicked()
{
    accept();
    emit requestDelete();
}

void DeleteDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));  // rgba = semi-transparent gray
}

void DeleteDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    printf("%s", mainWindow->objectName().toUtf8().data());
    printf("%d %d\n", parentRect.size().width(), parentRect.size().height());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}
