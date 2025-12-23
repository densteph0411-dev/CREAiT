#ifndef REVIEWERITEMWIDGET_H
#define REVIEWERITEMWIDGET_H

#include <QWidget>
#include "ui_revieweritemwidget.h"

#include <reviewscontroller.h>

class ReviewerItemWidget : public QWidget, public Ui::ReviewerItemWidget
{
    Q_OBJECT

public:
    explicit ReviewerItemWidget(QWidget *parent = nullptr);
    ~ReviewerItemWidget();

    void setData(int id, const ReviewerData& data);

signals:
    void requestReviewItemCheckResult(int id);

private slots:
    void clickedViewDetailButton();

private:
    int index;
};

#endif // REVIEWERITEMWIDGET_H
