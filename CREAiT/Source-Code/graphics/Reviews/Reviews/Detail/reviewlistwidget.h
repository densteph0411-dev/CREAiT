#ifndef REVIEWLISTWIDGET_H
#define REVIEWLISTWIDGET_H

#include <QList>
#include <QWidget>
#include <QVBoxLayout>
#include "ui_reviewlistwidget.h"
#include "reviewitemwidget.h"
#include "revieweritemwidget.h"
#include "checklistitemwidget.h"
#include <flowlayout.h>

#include <reviewscontroller.h>

enum {
    LIST_TYPE_REVIEW_ITEMS,
    LIST_TYPE_REVIEWERS,
    LIST_TYPE_CHECKLISTS,
    LIST_TYPE_STANDARDS
};

class ReviewListWidget : public QWidget, public Ui::ReviewListWidget
{
    Q_OBJECT

public:
    explicit ReviewListWidget(QWidget *parent = nullptr);
    ~ReviewListWidget();

    void initLayout();
    void initializeWidgets();
    void setReviewItemData(const QString& reviewId, const QMap<QString, ReviewItemData>& reviewItems);
    void setReviewerItemData(const QList<ReviewerData>& reviewerItems);
    void setChecklistItemData(const QList<int>& checklistItems);
    void setStandardItemData(const QList<int>& standardItems);

private:
    void addReviewItemWidget(const QString& itemId, const ReviewItemData& data);
    void addReviewerItemWidget(const ReviewerData& data);
    void addChecklistItemWidget(const ChecklistData& data);
    void addStandardItemWidget(const StandardData& data);

    void clearReviews();

signals:
    void requestReviewItemDetail(const QString& reviewId, const QString& reviewItemId);

private slots:
    void handlereviewItemCheckResult(int id);

private:
    QString parentId;
    int listType;
    QVBoxLayout* mainVLayout;
    FlowLayout* mainFlowLayout;
    QList<ReviewItemWidget*> reviewItemWidgets;
    QList<ReviewerItemWidget*> reviewerItemWidgets;
    QList<ChecklistItemWidget*> checklistItemWidgets;
};

#endif // REVIEWLISTWIDGET_H
