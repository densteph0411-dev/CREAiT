#ifndef REVIEWITEMCHECKRESULTWIDGET_H
#define REVIEWITEMCHECKRESULTWIDGET_H

#include <QWidget>
#include "ui_reviewitemcheckresultwidget.h"


class ReviewItemCheckResultWidget : public QWidget, public Ui::ReviewItemCheckResultWidget
{
    Q_OBJECT

public:
    explicit ReviewItemCheckResultWidget(QWidget *parent = nullptr);
    ~ReviewItemCheckResultWidget();

    void setData(const QString& quesiton, int id, const QString& description);
};

#endif // REVIEWITEMCHECKRESULTWIDGET_H
