#ifndef REVIEWSPAGE_H
#define REVIEWSPAGE_H

#include <QWidget>
#include "ui_reviewspage.h"


class ReviewsPage : public QWidget, public Ui::ReviewsPage
{
    Q_OBJECT

public:
    explicit ReviewsPage(QWidget *parent = nullptr);
    ~ReviewsPage();

    void initializeWidgets();
    void navigateToPage(int pageIndex);
    void handlePageRequest(int pageIndex, int cellId);
    void handleReiewPageReqeuest(int pageIndex, const QString& reviewId);
    void handleReviewItemRequest(const QString& reviewId, const QString& reviewItemId);
    void handleFindingRequest(const QString& reviewId, const QString& reviewItemId, const QString& findingId);

    void showPage(int index);

signals:
    void handleButtonSelect(int buttonIndex);

public:
    enum {
        REVIEWS_DASHBOARD_PAGE,
        STANDARD_DASHBOARD_PAGE,
        CHECKLIST_DASHBOARD_PAGE,
        CHECKLIST_CREATE_PAGE,
        REVIEW_CREATE_PAGE,
        REVIEW_DETAIL_PAGE,
        REVIEW_ITEM_DETAIL_PAGE,
        FINDING_PAGE,
    };
};

#endif // REVIEWSPAGE_H
