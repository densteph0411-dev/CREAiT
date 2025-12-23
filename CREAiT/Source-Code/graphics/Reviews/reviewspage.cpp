#include "reviewspage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QFrame>
#include <QIcon>
#include <QSize>
#include <QStackedWidget>
#include <QLabel>


ReviewsPage::ReviewsPage(QWidget *parent)
    : QWidget(parent) {
    initializeWidgets();
}

ReviewsPage::~ReviewsPage() {

};

void ReviewsPage::initializeWidgets() {
    setupUi(this);

    contextSideBar->setItemData(0, "Reviews", ":/images/Reviews/reviews.svg");
    contextSideBar->setItemData(1, "Standards", ":/images/Reviews/standards.svg");
    contextSideBar->setItemData(2, "Checklists", ":/images/Reviews/checklists.svg");
    contextSideBar->setItemData(3, "Reviews", ":/images/ProjectSettings/basic_settings.svg", true);

    connect(this, &ReviewsPage::handleButtonSelect, contextSideBar, &ContextSidebar::handleButtonSelect);
    connect(contextSideBar, &ContextSidebar::requestPage, this, &ReviewsPage::showPage);
    connect(reviewsViewPage, &ReviewsViewPage::requestCreate, this, &ReviewsPage::handleReiewPageReqeuest);
    connect(reviewsViewPage, &ReviewsViewPage::requestDetail, this, &ReviewsPage::handleReiewPageReqeuest);
    connect(checklistsViewPage, &ChecklistsViewPage::requestCreatePage, this, &ReviewsPage::handlePageRequest);
    connect(createChecklistPage, &CreateChecklistPage::requestBackPage, this, &ReviewsPage::showPage);
    connect(createReviewPage, &CreateReviewPage::requestBack, this, &ReviewsPage::showPage);
    connect(reviewDetailPage, &ReviewDetailPage::requestBack, this, &ReviewsPage::showPage);
    connect(reviewDetailPage, &ReviewDetailPage::requestEdit, this, &ReviewsPage::handleReiewPageReqeuest);
    connect(reviewDetailPage, &ReviewDetailPage::requestReviewItemDetail, this, &ReviewsPage::handleReviewItemRequest);
    connect(reviewItemDetailPage, &ReviewItemDetailPage::requestBack, this, &ReviewsPage::showPage);
    connect(reviewItemDetailPage, &ReviewItemDetailPage::requestFinding, this, &ReviewsPage::handleFindingRequest);
    connect(findingPage, &FindingPage::requestBack, this, &ReviewsPage::showPage);

    navigateToPage(0);
}

void ReviewsPage::navigateToPage(int index) {
    if (index < 0 || index > reviewsPageContainer->count() - 1) {
        qDebug() << "Invalid page index:" << index;
        return;
    }

    emit handleButtonSelect(index);
    reviewsPageContainer->setCurrentIndex(index);
}

void ReviewsPage::handlePageRequest(int pageIndex, int cellId) {
    if (pageIndex < 0 || pageIndex > reviewsPageContainer->count() - 1) {
        qDebug() << "Invalid page index:" << pageIndex;
        return;
    }

    switch(pageIndex) {
    case 3:
        createChecklistPage->setData(cellId);
        break;
    }

    showPage(pageIndex);
}

void ReviewsPage::handleReiewPageReqeuest(int pageIndex, const QString& reviewId) {
    if (pageIndex < 0 || pageIndex > reviewsPageContainer->count() - 1) {
        qDebug() << "Invalid page index:" << pageIndex;
        return;
    }

    switch(pageIndex) {
    case 4:
        createReviewPage->setData(reviewId);
        break;
    case 5:
        reviewDetailPage->setData(reviewId);
        break;
    }

    showPage(pageIndex);
}

void ReviewsPage::handleReviewItemRequest(const QString& reviewId, const QString& reviewItemId) {
    reviewItemDetailPage->setData(reviewId, reviewItemId);

    showPage(REVIEW_ITEM_DETAIL_PAGE);
}

void ReviewsPage::handleFindingRequest(const QString& reviewId, const QString& reviewItemId, const QString& findingId) {
    findingPage->setData(reviewId, reviewItemId, findingId);

    showPage(FINDING_PAGE);
}

void ReviewsPage::showPage(int index) {
    if (index < 0 || index > reviewsPageContainer->count() - 1) {
        qDebug() << "Invalid page index:" << index;
        return;
    }

    reviewsPageContainer->setCurrentIndex(index);
}


