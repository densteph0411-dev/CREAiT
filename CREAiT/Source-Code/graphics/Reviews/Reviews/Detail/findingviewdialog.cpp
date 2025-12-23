#include "findingviewdialog.h"
#include <QPainter>
#include "findingwidget.h"

FindingViewDialog::FindingViewDialog(QWidget *parent)
    : QDialog(parent)
{
    initializeWidgets();
}

FindingViewDialog::~FindingViewDialog() {

}

void FindingViewDialog::initializeWidgets() {
    setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(18);
    mainLayout->setAlignment(Qt::AlignTop);

    connect(closeButton, &QToolButton::clicked, this, &QDialog::reject);
}

void FindingViewDialog::setData(const QString& reviewId, const QString& reviewItemId, const QList<FindingData>& data) {
    for (int i = 0; i < data.count(); ++i) {
        FindingWidget* widget = new FindingWidget(container);
        widget->setFindingData(reviewId, reviewItemId, data.at(i));
        mainLayout->addWidget(widget);
    }

    constexpr int bodyExtra = 36;
    constexpr int dialogExtra = 64;
    constexpr int maxScrollHeight = 700;
    constexpr int maxBodyHeight = 736;
    constexpr int maxDialogHeight = 800;

    scrollArea->widget()->adjustSize();

    int scrollContentHeight = scrollArea->widget()->height();
    if (scrollContentHeight > maxScrollHeight) mainLayout->setContentsMargins(0, 0, 10, 0);
    int scrollHeight = qMin(scrollContentHeight, maxScrollHeight);

    scrollArea->setFixedHeight(scrollHeight);

    int bodyHeight = qMin(bodyExtra + scrollHeight, maxBodyHeight);
    bodyContainer->setFixedHeight(bodyHeight);

    int dialogHeight = qMin(dialogExtra + bodyHeight, maxDialogHeight);
    dialogContainer->setFixedHeight(dialogHeight);
}

void FindingViewDialog::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void FindingViewDialog::showEvent(QShowEvent *event) {
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}
