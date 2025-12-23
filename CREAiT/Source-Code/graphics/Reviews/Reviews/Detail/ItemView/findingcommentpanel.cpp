#include "findingcommentpanel.h"
#include <findingwidget.h>
#include "findingdialog.h"
#include "reviewactiondialog.h"
#include <QDateTime>

FindingCommentPanel::FindingCommentPanel(QWidget *parent)
    : QWidget(parent)
{
    initializeWidgets();
}

FindingCommentPanel::~FindingCommentPanel() {
    clearLayout();
}

void FindingCommentPanel::initializeWidgets() {
    setupUi(this);

    containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->setSpacing(12);

    scrollArea->hide();

    connect(addButton, &QPushButton::clicked, this, &FindingCommentPanel::clickedAddButton);
}

void FindingCommentPanel::clearLayout() {
    QLayoutItem *item;
    while ((item = containerLayout->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

void FindingCommentPanel::clear(int type) {
    panelType = type;

    clearLayout();

    if (panelType == FINDING) {
        headerIcon->setIcon(QIcon(":images/Reviews/finding.svg"));
        headerLabel->setText("Findings");
        addButton->setText("Create Finding");
        noItemLabel->setText("No findings yet");
    } else {
        headerIcon->setIcon(QIcon(":images/Reviews/activity.svg"));
        headerLabel->setText("Activities and Comments");
        addButton->setText("Add Comment");
        noItemLabel->setText("No activities and comments yet");
    }
}

void FindingCommentPanel::setFindingData(const QString& parentId, const QString& itemId) {
    reviewId = parentId;
    reviewItemId = itemId;
    panelType = FINDING;

    clear(panelType);

    const QList<FindingData> findings = ReviewsController::getFindings(reviewId, reviewItemId);
    for (const FindingData& finding: findings) addFinding(finding, false);

    adjustScrollHeight();
}
void FindingCommentPanel::setCommentData(const QString& parentId, const QString& itemId, const QString& subItemId) {
    reviewId = parentId;
    reviewItemId = itemId;
    findingId = subItemId;
    panelType = COMMENT;

    clear(panelType);
    const QList<CommentData> comments = ReviewsController::getComments(reviewId, reviewItemId, findingId);
    for (const CommentData& comment: comments) addComment(comment, false);

    adjustScrollHeight();
}

void FindingCommentPanel::clickedAddButton() {
    if (panelType == FINDING) {

        auto item = ReviewsController::getReviewItem(reviewId, reviewItemId);
        if (!item) return;
        const ReviewItemData& data = item.value();

        FindingDialog* dialog = new FindingDialog();
        dialog->setData(data.item);
        int result = dialog->exec();
        if (result == QDialog::Rejected) return;

        FindingData findingData;
        findingData.id = ReviewsController::generateUniqueId();
        findingData.assignee = dialog->assigneeSelect->currentText();
        findingData.verifier = dialog->verifierSelect->currentText();
        findingData.description = dialog->descriptionEdit->toPlainText();
        findingData.createdAt = QDateTime::currentDateTime().toString("MMM d, yyyy");
        findingData.status = "Implemented";
        findingData.finder = "Tim Trvis";
        findingData.statusReason = "";

        ReviewsController::addFindingData(reviewId, reviewItemId, findingData);
        addFinding(findingData);
    } else {
        ReviewActionDialog* dialog = new ReviewActionDialog();
        dialog->setData(1, 1, -1, "", "");
        int result = dialog->exec();
        if (result == QDialog::Rejected) return;

        CommentData data;
        data.id = ReviewsController::generateUniqueId();
        data.writer = "Tim Trvis";
        data.content = dialog->descriptionEdit->toPlainText();
        data.createdAt = QDateTime::currentDateTime().toString("MMM d, yyyy");
        ReviewsController::addCommentData(reviewId, reviewItemId, findingId, data);

        addComment(data);
    }
}

void FindingCommentPanel::addFinding(const FindingData& data, bool adjustHeight) {
    if (containerLayout->count() < 1) {
        noItemBox->hide();
        scrollArea->show();
        scrollArea->setFixedHeight(0);
    }

    FindingWidget* widget = new FindingWidget(container);
    widget->setFindingData(reviewId, reviewItemId, data);
    connect(widget, &FindingWidget::requestFinding, this, &FindingCommentPanel::requestFinding);

    containerLayout->addWidget(widget);

    if (adjustHeight) {
        scrollArea->widget()->adjustSize();
        scrollArea->setFixedHeight(scrollArea->height() + widget->height());
    }
}

void FindingCommentPanel::addComment(const CommentData& data, bool adjustHeight) {
    if (containerLayout->count() < 1) {
        noItemBox->hide();
        scrollArea->show();
        scrollArea->setFixedHeight(0);
    }

    FindingWidget* widget = new FindingWidget(container);
    widget->setCommentData(data);
    widget->Header->hide();
    widget->commentButton->hide();
    widget->adjustSize();

    containerLayout->addWidget(widget);

    if (adjustHeight) {
        scrollArea->widget()->adjustSize();
        scrollArea->setFixedHeight(scrollArea->height() + widget->height() + 12);
    }
}

void FindingCommentPanel::adjustScrollHeight() {
    scrollArea->widget()->adjustSize();
    scrollArea->setFixedHeight(containerLayout->sizeHint().height());
}
