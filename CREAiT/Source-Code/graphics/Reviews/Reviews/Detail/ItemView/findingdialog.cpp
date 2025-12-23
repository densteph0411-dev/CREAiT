#include "findingdialog.h"
#include <QPainter>

FindingDialog::FindingDialog(QWidget *parent)
    : QDialog(parent)
{
    initializeWidgets();
}

FindingDialog::~FindingDialog()
{

}

void FindingDialog::initializeWidgets() {
    setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    versionSelect->addItems({
        "2.4.6.91",
        "2.4.6.93",
        "2.4.6.95",
        "2.4.6.96",
        "2.4.6.99",
    });

    verifierSelect->addItems({ "Tim Trvis", "James William", "Johnny Wick", "Michal Joe" });
    assigneeSelect->addItems({ "Tim Trvis", "James William", "Johnny Wick", "Michal Joe" });

    connect(closeButton, &QToolButton::clicked, this, &QDialog::reject);
    connect(cancelButton, &QToolButton::clicked, this, &QDialog::reject);
    connect(saveButton, &QToolButton::clicked, this, &QDialog::accept);
}

void FindingDialog::setData(const QString& reviewItemName) {
    reviewItemEdit->setText(reviewItemName);
}

void FindingDialog::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void FindingDialog::showEvent(QShowEvent *event) {
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}
