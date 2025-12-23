#include "logitemwidget.h"

LogItemWidget::LogItemWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
}

LogItemWidget::~LogItemWidget()
{

}

QString LogItemWidget::getMessage() const {
    return messageLabel->text();
}

QString LogItemWidget::getTime() const {
    return timeLabel->text();
}
