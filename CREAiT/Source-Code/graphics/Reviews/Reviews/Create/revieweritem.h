#ifndef REVIEWERITEM_H
#define REVIEWERITEM_H

#include "ui_revieweritem.h"
#include <QWidget>
#include <reviewscontroller.h>

class ReviewerItem : public QWidget, public Ui::ReviewerItem
{
    Q_OBJECT

public:
    explicit ReviewerItem(int id, QWidget *parent = nullptr);
    ~ReviewerItem();

    void initializeWidgets();
    ReviewerData getData() const;
    void setData(const ReviewerData& data);
    void refresh();
protected:
    void showEvent(QShowEvent* event);

signals:
    void requestReviewItemCheck(int id);
    void requestDelete(int id);

private slots:
    void clickedReviewItemCheckButton();
    void clickedDeleteButton();

private:
    int index;

};

#endif // REVIEWERITEM_H
