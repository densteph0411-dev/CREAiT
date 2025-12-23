#include "checklistviewdialog.h"
#include <QPainter>
#include <QSizePolicy>
#include <reviewscontroller.h>

ChecklistViewDialog::ChecklistViewDialog(QWidget *parent)
    : QDialog(parent)
{
    initializeWidgets();
}

ChecklistViewDialog::~ChecklistViewDialog() {

}

void ChecklistViewDialog::initializeWidgets() {
    setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(18, 18, 18, 18);
    mainLayout->setSpacing(18);

    connect(closeButton, &QToolButton::clicked, this, &QDialog::reject);
}

void ChecklistViewDialog::setData(const QString& title, const QStringList& questions) {
    dialogTitle->setText(title);

    for (int i = 0; i < questions.count(); ++i) {
        QLabel* label = new QLabel();
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
        label->setMaximumHeight(48);
        label->setWordWrap(true);
        label->setStyleSheet(R"(
            font-family: Open Sans;
            font-weight: 400;
            font-size: 16px;
            line-height: 24px;
            letter-spacing: 0%;
            color: #1A1A1A;
        )");

        QString question = QString("%1. %2").arg(i + 1).arg(questions.at(i));
        label->setText(question);
        mainLayout->addWidget(label);
    }
}

void ChecklistViewDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void ChecklistViewDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}
