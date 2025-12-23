#ifndef LOGITEMWIDGET_H
#define LOGITEMWIDGET_H

#include <QWidget>
#include "ui_logitemwidget.h"

class LogItemWidget : public QWidget, public Ui::LogItemWidget
{
    Q_OBJECT

public:
    explicit LogItemWidget(QWidget *parent = nullptr);
    ~LogItemWidget();

    QString getMessage() const;
    QString getTime() const;
};

#endif // LOGITEMWIDGET_H
