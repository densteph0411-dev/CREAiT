#ifndef REVIEWITEMWIDGET_H
#define REVIEWITEMWIDGET_H

#include <QWidget>
#include "ui_reviewitemwidget.h"
#include <reviewscontroller.h>

class ReviewItemWidget : public QWidget, public Ui::ReviewItemWidget
{
    Q_OBJECT

public:
    explicit ReviewItemWidget(QWidget *parent = nullptr);
    ~ReviewItemWidget();

    void initializeWidgets();
    void setData(const QString& id, const ReviewItemData& data);

signals:
    void requestDetail(const QString& parentId, const QString& itemId);


private slots:
    void clickedShowFindingButton();
    void clickedViewDetailButton();

private:
    QString reviewId;
    QString reviewItemId;
    ReviewItemData currentData;
};

#endif // REVIEWITEMWIDGET_H
