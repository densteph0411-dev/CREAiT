#include "automationtestresultdialog.h"
#include <QPainter>

AutomationTestResultDialog::AutomationTestResultDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    QString html = R"(
        <ul style="font-family: Open Sans; font-size: 20px; line-height: 1.4;">
            <li style="margin-bottom: 15px;">Reading test documents</li>

            <li style="margin-bottom: 15px;">Reading test records</li>

            <li style="margin-bottom: 15px;">LCreating dummy test cases</li>

            <li style="color: #0D92F4;">Automation successful</li>
        </ul>
    )";

    bodyTextViewer->setHtml(html);
    bodyTextViewer->setReadOnly(true);

    connect(closeButton, &QToolButton::clicked, this, &AutomationTestResultDialog::close);
}

AutomationTestResultDialog::~AutomationTestResultDialog()
{

}

void AutomationTestResultDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));  // rgba = semi-transparent gray
}

void AutomationTestResultDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}
