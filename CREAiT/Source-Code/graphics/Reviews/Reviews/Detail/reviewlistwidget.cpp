#include "reviewlistwidget.h"
#include <QLayoutItem>
#include <hseparatorwidget.h>
#include <standardcell.h>
#include "reviewitemcheckresultdialog.h"

ReviewListWidget::ReviewListWidget(QWidget *parent)
    : QWidget(parent), listType(LIST_TYPE_REVIEW_ITEMS)
{
    initializeWidgets();
}

ReviewListWidget::~ReviewListWidget() {

}

void ReviewListWidget::initLayout() {
    switch(listType) {
    case LIST_TYPE_REVIEW_ITEMS:
    case LIST_TYPE_REVIEWERS:
    case LIST_TYPE_CHECKLISTS:
        body->setLayout(mainVLayout);
        break;
    case LIST_TYPE_STANDARDS:
        body->setLayout(mainFlowLayout);
        break;
    }
}

void ReviewListWidget::initializeWidgets() {
    setupUi(this);

    mainVLayout = new QVBoxLayout();
    mainVLayout->setContentsMargins(0, 10, 0, 10);
    mainVLayout->setSpacing(0);
    mainVLayout->setAlignment(Qt::AlignTop);

    mainFlowLayout = nullptr;
}

void ReviewListWidget::setReviewItemData(const QString& reviewId, const QMap<QString, ReviewItemData>& reviewItems) {
    parentId = reviewId;
    listType = LIST_TYPE_REVIEW_ITEMS;
    initLayout();
    headerLabel->setText("Review Items");

    clearReviews();

    for (auto it = reviewItems.cbegin(); it != reviewItems.cend(); ++it) {
        const QString& itemId = it.key();
        const ReviewItemData& itemData = it.value();

        addReviewItemWidget(itemId, itemData);
    }
}

void ReviewListWidget::setReviewerItemData(const QList<ReviewerData>& reviewerItems) {
    listType = LIST_TYPE_REVIEWERS;
    initLayout();
    headerLabel->setText("Reviewers");

    clearReviews();

    for (const auto& item : reviewerItems) {
        addReviewerItemWidget(item);
    }
}

void ReviewListWidget::setChecklistItemData(const QList<int>& checklistItems) {
    listType = LIST_TYPE_CHECKLISTS;
    initLayout();
    headerLabel->setText("Checklists");

    clearReviews();

    for (int id: checklistItems) {
        const ChecklistData& data = ReviewsController::getChecklistById(id);
        addChecklistItemWidget(data);
    }
}

void ReviewListWidget::setStandardItemData(const QList<int>& standardItems) {
    listType = LIST_TYPE_STANDARDS;

    if (mainFlowLayout) delete mainFlowLayout;
    mainFlowLayout = new FlowLayout(body, 10, 10);

    headerLabel->setText("Standards");

    for (int id: standardItems) {
        const StandardData& data = ReviewsController::getStandardById(id);
        StandardCell* cell = new StandardCell(id);
        cell->setData(data.title, data.filePath);
        cell->menuButton->hide();

        // connect(cell, &StandardCell::requestEdit, this, &StandardsViewPage::editStandard);
        // connect(cell, &StandardCell::requestRemove, this, &StandardsViewPage::deleteStandard);

        mainFlowLayout->addWidget(cell);
    }
    mainFlowLayout->update();
}

void ReviewListWidget::addReviewItemWidget(const QString& itemId, const ReviewItemData& data) {
    ReviewItemWidget* widget = new ReviewItemWidget();
    widget->setData(itemId, data);

    if (reviewItemWidgets.count() > 0) {
        HSeparatorWidget* separator = new HSeparatorWidget(body);
        mainVLayout->addWidget(separator);
    }

    mainVLayout->addWidget(widget);
    reviewItemWidgets.append(widget);

    connect(widget, &ReviewItemWidget::requestDetail, this, &ReviewListWidget::requestReviewItemDetail);
}

void ReviewListWidget::addReviewerItemWidget(const ReviewerData& data) {
    int id = reviewerItemWidgets.count() + 1;
    ReviewerItemWidget* widget = new ReviewerItemWidget();
    widget->setData(id, data);

    if (reviewerItemWidgets.count() > 0) {
        HSeparatorWidget* separator = new HSeparatorWidget(body);
        mainVLayout->addWidget(separator);
    }

    mainVLayout->addWidget(widget);
    reviewerItemWidgets.append(widget);

    connect(widget, &ReviewerItemWidget::requestReviewItemCheckResult, this, &ReviewListWidget::handlereviewItemCheckResult);
}

void ReviewListWidget::addChecklistItemWidget(const ChecklistData& data) {
    int id = checklistItemWidgets.count() + 1;
    ChecklistItemWidget* widget = new ChecklistItemWidget();
    widget->setData(id, data);

    if (checklistItemWidgets.count() > 0) {
        HSeparatorWidget* separator = new HSeparatorWidget(body);
        mainVLayout->addWidget(separator);
    }

    mainVLayout->addWidget(widget);
    checklistItemWidgets.append(widget);
}

void ReviewListWidget::addStandardItemWidget(const StandardData& data) {
    StandardCell* cell = new StandardCell(-1);
    cell->setData(data.title, data.filePath);
    cell->menuButton->hide();

    mainFlowLayout->addWidget(cell);
}

void ReviewListWidget::clearReviews() {
    QLayoutItem* item = nullptr;

    while ((item = mainVLayout->takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        if (widget) {
            widget->setParent(nullptr);
            widget->deleteLater();
        }
        delete item;
    }

    body->adjustSize();

    // if (listType == LIST_TYPE_STANDARDS) mainFlowLayout->invalidate();

    if (reviewItemWidgets.count() > 0) reviewItemWidgets.clear();
    if (reviewerItemWidgets.count() > 0) reviewerItemWidgets.clear();
    if (checklistItemWidgets.count() > 0) checklistItemWidgets.clear();
}

void ReviewListWidget::handlereviewItemCheckResult(int id) {
    ReviewItemCheckResultDialog* dialog = new ReviewItemCheckResultDialog();
    dialog->setData(id);
    dialog->show();
}
