#include "reviewdetailpage.h"
#include <reviewscontroller.h>

ReviewDetailPage::ReviewDetailPage(QWidget *parent)
    : QWidget(parent)
{
    initializeWidgets();
}

ReviewDetailPage::~ReviewDetailPage()
{

}

void ReviewDetailPage::initializeWidgets() {
    setupUi(this);

    createdOn->titleLabel->setText("Created On");
    createdOn->icon->setIcon(QIcon(":/images/Automations/cell_footer_createdon.svg"));

    createdBy->titleLabel->setText("Created By");
    createdBy->icon->setIcon(QIcon(":/images/Automations/cell_footer_createdby.svg"));

    reviewLeader->titleLabel->setText("Review Leader");
    reviewLeader->icon->setIcon(QIcon(":/images/Reviews/review_leader.svg"));

    primaryReviewer->titleLabel->setText("Primary Reviewer");
    primaryReviewer->icon->setIcon(QIcon(":/images/Reviews/primary_reviewer.svg"));


    connect(backButton, &QPushButton::clicked, this, &ReviewDetailPage::clickedBackButton);
    connect(editButton, &QPushButton::clicked, this, &ReviewDetailPage::clickedEditButton);
    connect(saveButton, &QPushButton::clicked, this, &ReviewDetailPage::clickedSaveButton);

    connect(reviewListWidget, &ReviewListWidget::requestReviewItemDetail, this, [this](const QString& reviewId, const QString& reviweItemId) {
        emit requestReviewItemDetail(reviewId, reviweItemId);
    });
}

void ReviewDetailPage::setData(const QString& id) {
    reviewId = id;

    auto item = ReviewsController::getReviewById(id);
        if (!item) return;

    const ReviewData& data = item.value();
    createdBy->description->setText(data.name);
    createdOn->description->setText(data.dueDate);
    reviewLeader->description->setText(data.reviewLeader);
    primaryReviewer->description->setText(data.primaryReviewer);

    reviewListWidget->setReviewItemData(reviewId, data.reviewItems);
    reviewerListWidget->setReviewerItemData(data.reviewers);
    checklistWidget->setChecklistItemData(data.checklists);
    standardListWidget->setStandardItemData(data.standards);
}


void ReviewDetailPage::clickedBackButton() {
    emit requestBack(0);
}

void ReviewDetailPage::clickedEditButton() {
    emit requestEdit(4, reviewId);
}

void ReviewDetailPage::clickedSaveButton() {
    emit requestBack(0);
}
