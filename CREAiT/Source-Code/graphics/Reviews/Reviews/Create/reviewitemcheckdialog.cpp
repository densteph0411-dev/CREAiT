#include "reviewitemcheckdialog.h"
#include <QPainter>

ReviewItemCheckDialog::ReviewItemCheckDialog(QWidget *parent)
    : QDialog(parent), index(-1)
{
    initializeWidgets();
}

ReviewItemCheckDialog::~ReviewItemCheckDialog() {

}

void ReviewItemCheckDialog::initializeWidgets() {
    setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(0, 0, 10, 0);
    mainLayout->setSpacing(18);

    connect(closeButton, &QPushButton::clicked, this, &ReviewItemCheckDialog::clickedCloseButton);
    connect(cancelButton, &QPushButton::clicked, this, &ReviewItemCheckDialog::clickedCancelButton);
    connect(saveButton, &QPushButton::clicked, this, &ReviewItemCheckDialog::clickedSaveButton);
}

void ReviewItemCheckDialog::setData(int id) {
    index = id;

    QString title = "What is the main goal of this project?";
    int checkedId = 2;
    QString description = "The goal of this project is not exactly confirmed yet, but we can say shortly. What is the main issue of this project? How long will it take to finish this project?";

    for (int i = 0; i < 5; ++i) {
        ReviewItemCheckWidget* widget = new ReviewItemCheckWidget(this);
        widget->setData(title, checkedId, description);

        mainLayout->addWidget(widget);
    }
}

void ReviewItemCheckDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void ReviewItemCheckDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}

void ReviewItemCheckDialog::clickedCloseButton() {
    reject();
}

void ReviewItemCheckDialog::clickedCancelButton() {
    reject();
}

void ReviewItemCheckDialog::clickedSaveButton() {
    accept();
}
