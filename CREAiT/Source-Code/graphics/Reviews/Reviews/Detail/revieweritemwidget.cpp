#include "revieweritemwidget.h"

ReviewerItemWidget::ReviewerItemWidget(QWidget *parent)
    : QWidget(parent), index(-1)
{
    setupUi(this);

    connect(viewDetailButton, &QPushButton::clicked, this, &ReviewerItemWidget::clickedViewDetailButton);
}

ReviewerItemWidget::~ReviewerItemWidget() {

}

void ReviewerItemWidget::setData(int id, const ReviewerData& data) {
    index = id;

    reviewerValueLabel->setText(data.name);
    roleValueLabel->setText(reviewerRoles[data.role]);
}

void ReviewerItemWidget::clickedViewDetailButton() {
    emit requestReviewItemCheckResult(index);
}
