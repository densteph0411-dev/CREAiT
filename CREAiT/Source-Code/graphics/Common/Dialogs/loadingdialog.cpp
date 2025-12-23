#include "loadingdialog.h"
#include <QPainter>


LoadingDialog::LoadingDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    spinner = new SpinnerWidget(dialogContainer);
    spinner->resize(dialogContainer->size());
    spinner->move(0, 0);
    spinner->raise();        // Make sure it's on top
    spinner->start();
}

LoadingDialog::~LoadingDialog() {
    spinner->stop();
    delete spinner;
}

void LoadingDialog::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));  // rgba = semi-transparent gray
}

void LoadingDialog::showEvent(QShowEvent *event) {
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}
