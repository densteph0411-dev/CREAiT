#include "findingwidget.h"
#include <QMouseEvent>

FindingWidget::FindingWidget(QWidget *parent)
    : QWidget(parent)
{
    initializeWidgets();
}

FindingWidget::~FindingWidget() {

}

void FindingWidget::initializeWidgets() {
    setupUi(this);

    containerLayout->setAlignment(Qt::AlignTop);
}

void FindingWidget::setFindingData(const QString& parentId, const QString& itemId, const FindingData& data) {
    reviewId = parentId;
    reviewItemId = itemId;
    findingId = data.id;

    const QList<CommentData>& comments = ReviewsController::getComments(reviewId, reviewItemId, findingId);

    findingIDValueLabel->setText(data.id);
    assigneeValueLabel->setText(data.assignee);
    verifyButton->setText(data.status);
    finderLabel->setText(data.finder);
    dueDateLabel->setText(data.createdAt);
    descriptionLabel->setText(data.description);
    commentButton->setText(QString("%1 comments").arg(comments.count()));

    if (data.assignee.isEmpty()) {
        AssigneeBox->hide();
        verifyButton->setText("Implemented");
    }

    if (data.status == "Verified") assigneeButton->hide();
}

void FindingWidget::setCommentData(const CommentData& data) {
    commentId = data.id;

    finderLabel->setText(data.writer);
    dueDateLabel->setText(data.createdAt);
    descriptionLabel->setText(data.content);
}

void FindingWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit requestFinding(reviewId, reviewItemId, findingId);
    }
}
