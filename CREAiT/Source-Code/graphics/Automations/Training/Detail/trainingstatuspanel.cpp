#include "trainingstatuspanel.h"
#include <QDesktopServices>
#include <QStyleOption>
#include <QPainter>
#include <logmanager.h>

TrainingStatusPanel::TrainingStatusPanel(QWidget *parent)
    : QWidget(parent)
{
    initializeWidgets();
    appendExampleLogs();

    connect(logDownloadButton, &QPushButton::clicked, this, &TrainingStatusPanel::clickedLogDownloadButton);
}

TrainingStatusPanel::~TrainingStatusPanel() {

}

void TrainingStatusPanel::initializeWidgets() {
    setupUi(this);

    logItems = new QList<LogItemWidget*>;
    logItemContainerLayout = new QVBoxLayout(logItemContainer);
    logItemContainerLayout->setAlignment(Qt::AlignTop);
    logItemContainerLayout->setContentsMargins(0, 0, 0, 0);
    logItemContainerLayout->setSpacing(20);
}

void TrainingStatusPanel::appendLog(const QString& message, const QString& time) {
    LogItemWidget* itemWidget = new LogItemWidget();
    itemWidget->messageLabel->setText(message);
    itemWidget->timeLabel->setText(time);

    logItemContainerLayout->addWidget(itemWidget);
    logItems->append(itemWidget);
}

void TrainingStatusPanel::appendExampleLogs() {
    QString message1 = "Training has been started.";
    QString time1 = "00:58:00";
    QString message2 = "Training is in progress";
    QString time2 = "00:58:24";
    QString message3 = "Lorem ipsum dolor sit amet consectetur. Purus habitasse ut scelerisque netus magna.. Eu semper porttitor nibh nunc scelerisque dictumst.. Risus in at nibh tristique. Nulla vulputate tempus dignissim lorem. Ullamcorper a libero tempus sodales volutpat fames in elementum..";
    QString time3 = "00:58:32";

    appendLog(message1, time1);
    appendLog(message2, time2);
    appendLog(message3, time3);
}

void TrainingStatusPanel::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void TrainingStatusPanel::clickedLogDownloadButton() {
    if (logItems->size() < 1) return;

    for (int i = 0; i < logItems->size(); ++i) {
        auto logItem = logItems->at(i);
        LogManager::instance().writeLog(logItem->getMessage(), logItem->getTime());
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(LogManager::instance().getLogFilePath()));
}
