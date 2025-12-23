#include "adddocumentdialog.h"
#include <QPainter>
#include <QApplication>
#include <QMainWindow>

AddDocumentDialog::AddDocumentDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);
}

AddDocumentDialog::~AddDocumentDialog()
{

}

QString AddDocumentDialog::getDocumentName() const
{
    return documentNameEdit->text();
}

QString AddDocumentDialog::getPurpose() const
{
    return purposeEdit->text();
}

QString AddDocumentDialog::getDescription() const
{
    return descriptionEdit->toPlainText();
}

void AddDocumentDialog::onSaveButtonClicked()
{
    accept();
}

void AddDocumentDialog::onCancelButtonClicked()
{
    reject();
}

void AddDocumentDialog::onCloseButtonClicked()
{
    reject();
}

void AddDocumentDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void AddDocumentDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = nullptr;
    
    // First try to get the main window from parent chain
    QWidget *parent = parentWidget();
    while (parent) {
        if (qobject_cast<QMainWindow*>(parent)) {
            mainWindow = parent;
            break;
        }
        parent = parent->parentWidget();
    }
    
    // Fallback to activeWindow if not found in parent chain
    if (!mainWindow) {
        mainWindow = QApplication::activeWindow();
    }
    
    if (mainWindow) {
        QRect parentRect = mainWindow->rect();
        QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
        setFixedSize(parentRect.size());
        move(topLeft);
    }

    QDialog::showEvent(event);
}

