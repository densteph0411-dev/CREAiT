#include "revieweritem.h"
#include <reviewscontroller.h>

ReviewerItem::ReviewerItem(int id, QWidget *parent)
    : QWidget(parent), index(id)
{
    initializeWidgets();
}

ReviewerItem::~ReviewerItem() {

}

void ReviewerItem::initializeWidgets() {
    setupUi(this);

    reviewerSelect->addItems(tempReviewers);
    reviewerRoleSelect->addItems(reviewerRoles);

    connect(checkButton, &QPushButton::clicked, this, &ReviewerItem::clickedReviewItemCheckButton);
    connect(deleteButton, &QPushButton::clicked, this, &ReviewerItem::clickedDeleteButton);
}

ReviewerData ReviewerItem::getData() const {
    ReviewerData data;
    data.name = reviewerSelect->currentText();
    data.role = reviewerRoleSelect->currentIndex();

    return data;
}

void ReviewerItem::setData(const ReviewerData& data) {
    refresh();

    reviewerSelect->setCurrentText(data.name);
    reviewerRoleSelect->setCurrentIndex(data.role);
}

void ReviewerItem::refresh() {
    reviewerSelect->clear();
    reviewerRoleSelect->clear();

    reviewerSelect->addItems(tempReviewers);
    reviewerRoleSelect->addItems(reviewerRoles);
}

void ReviewerItem::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    // refresh();
}

void ReviewerItem::clickedReviewItemCheckButton() {
    emit requestReviewItemCheck(index);
}

void ReviewerItem::clickedDeleteButton() {
    emit requestDelete(index);
}
