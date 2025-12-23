#include "createreviewpage.h"
#include "checklistdialog.h"
#include "reviewscontroller.h"
#include <standardcreatedialog.h>
#include "reviewitemcheckdialog.h"

CreateReviewPage::CreateReviewPage(QWidget *parent)
    : QWidget(parent), reviewItemNextId(0), reviewerItemNextId(0), requestItemNextId(0)
{
    initializeWidgets();
}

CreateReviewPage::~CreateReviewPage() {

}

void CreateReviewPage::initializeWidgets() {
    setupUi(this);

    auto setupItemLayout = [](QWidget* container) {
        QVBoxLayout* layout = new QVBoxLayout(container);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(16);
        return layout;
    };

    reviewLeaderSelect->addItems(tempReviewers);
    primaryReviewerSelect->addItems(tempReviewers);

    reviewItemLayout   = setupItemLayout(reviewItemContainer);
    reviewerItemLayout = setupItemLayout(reviewerItemContainer);
    requestItemLayout  = setupItemLayout(requestItemContainer);

    connect(addReviewItemButton, &QPushButton::clicked, this, &CreateReviewPage::clickedAddReviewItemButton);
    connect(addReviewerItemButton, &QPushButton::clicked, this, &CreateReviewPage::clickedAddReviewerItemButton);
    connect(addRequestItemButton, &QPushButton::clicked, this, &CreateReviewPage::clickedAddRequestItemButton);

    connect(addChecklistButton, &QPushButton::clicked, this, &CreateReviewPage::clickedAddChecklistButton);
    connect(addStandardButton, &QPushButton::clicked, this, &CreateReviewPage::clickedAddStandardButton);

    connect(backButton, &QPushButton::clicked, this, &CreateReviewPage::clickedBackButton);
    connect(cancelButton, &QPushButton::clicked, this, &CreateReviewPage::clickedCancelButton);
    connect(saveButton, &QPushButton::clicked, this, &CreateReviewPage::clickedSaveButton);

    addChecklistButton->hide();
    addStandardButton->hide();
}

void CreateReviewPage::setData(const QString& id) {
    clear();

    reviewId = id;

    backButton->setText(reviewId.isEmpty() ? "Create Review" : "Edit Review");

    const auto checklists = ReviewsController::getChecklists();
    for (const auto& checklist : checklists) {
        checklistSelect->addItem(checklist.title);
    }

    const auto standards = ReviewsController::getStandards();
    for (const auto& standard : standards) {
        standardSelect->addItem(standard.title);
    }

    if (reviewId.isEmpty()) {
        emit clickedAddReviewItemButton();
        emit clickedAddReviewerItemButton();
        emit clickedAddRequestItemButton();
    } else {
        auto item = ReviewsController::getReviewById(id);
        if (!item) return;

        const ReviewData& data = item.value();
        reviewNameEdit->setText(data.name);
        QDate date = QDate::fromString(data.dueDate, "MMM d, yyyy");
        dueDateEdit->setDate(date);
        reviewLeaderSelect->setCurrentText(data.reviewLeader);
        primaryReviewerSelect->setCurrentText(data.primaryReviewer);

        // Review Items
        for (const ReviewItemData& itemData : data.reviewItems) {
            emit clickedAddReviewItemButton();
            int lastId = reviewItemNextId - 1;
            if (reviewItems.contains(lastId)) {
                ReviewItem* item = reviewItems[lastId];
                item->setData(itemData);
            }
        }

        // Reviewer Items
        for (const ReviewerData& reviewer : data.reviewers) {
            emit clickedAddReviewerItemButton();
            int lastId = reviewerItemNextId - 1;
            if (reviewerItems.contains(lastId)) {
                ReviewerItem* item = reviewerItems[lastId];
                item->setData(reviewer); // assumes ReviewerItem has setData(const ReviewerData&)
            }
        }

        // Checklist items
        for (int id: data.checklists) {
            QString title = ReviewsController::getChecklistTitle(id);
            checklistSelect->setCurrentItem(title);
        }

        // Standard items
        for (int id: data.standards) {
            QString title = ReviewsController::getStandardTitle(id);
            standardSelect->setCurrentItem(title);
        }

        // Request Items
        for (const RequestData& request : data.requests) {
            emit clickedAddRequestItemButton();
            int lastId = requestItemNextId - 1;
            if (requestItems.contains(lastId)) {
                RequestItem* item = requestItems[lastId];
                item->setData(request); // assumes RequestItem has setData(const RequestData&)
            }
        }
    }
}

void CreateReviewPage::removeAllReviewItems() {
    for (auto item : reviewItems) {
        reviewItemLayout->removeWidget(item);
        item->deleteLater();
    }
    reviewItems.clear();
    reviewItemNextId = 0;
}

void CreateReviewPage::removeAllReviewerItems() {
    for (auto item : reviewerItems) {
        reviewerItemLayout->removeWidget(item);
        item->deleteLater();
    }
    reviewerItems.clear();
    reviewerItemNextId = 0;
}

void CreateReviewPage::removeAllRequestItems() {
    for (auto item : requestItems) {
        requestItemLayout->removeWidget(item);
        item->deleteLater();
    }
    requestItems.clear();
    requestItemNextId = 0;
}

void CreateReviewPage::clear() {
    reviewNameEdit->setText("");
    reviewLeaderSelect->setCurrentText("");
    primaryReviewerSelect->setCurrentText("");

    removeAllReviewItems();
    removeAllReviewerItems();
    removeAllRequestItems();

    checklistSelect->clear();
    standardSelect->clear();
}

void CreateReviewPage::clickedAddReviewItemButton() {
    int id = reviewItemNextId++;
    ReviewItem* item = new ReviewItem(id, reviewItemContainer);
    reviewItemLayout->addWidget(item);
    reviewItems.insert(id, item);

    reviewItemContainer->adjustSize();
    ReviewPanel->adjustSize();

    connect(item, &ReviewItem::requestDelete, this, &CreateReviewPage::handleDeleteReviewItem);
}

void CreateReviewPage::clickedAddReviewerItemButton() {
    int id = reviewerItemNextId++;
    ReviewerItem* item = new ReviewerItem(id, reviewerItemContainer);
    reviewerItemLayout->addWidget(item);
    reviewerItems.insert(id, item);

    reviewerItemContainer->adjustSize();
    ReviewerPanel->adjustSize();

    connect(item, &ReviewerItem::requestReviewItemCheck, this, &CreateReviewPage::handleReviewItemCheck);
    connect(item, &ReviewerItem::requestDelete, this, &CreateReviewPage::handleDeleteReviewerItem);
}

void CreateReviewPage::clickedAddRequestItemButton() {
    int id = requestItemNextId++;
    RequestItem* item = new RequestItem(id, reviewItemContainer);
    requestItemLayout->addWidget(item);
    requestItems.insert(id, item);

    requestItemContainer->adjustSize();
    RequestPanel->adjustSize();

    connect(item, &RequestItem::requestDelete, this, &CreateReviewPage::handleDeleteRequestItem);
}

void CreateReviewPage::handleDeleteReviewItem(int id) {
    if (reviewItems.size() <= 1) return;

    if (reviewItems.contains(id)) {
        auto* item = reviewItems.take(id);
        if (item) {
            reviewItemLayout->removeWidget(item);
            item->deleteLater();
        }
    }

    reviewItemContainer->adjustSize();
}

void CreateReviewPage::handleReviewItemCheck(int id) {
    if (reviewerItems.size() <= 1 || id < 0) return;

    ReviewItemCheckDialog* dialog = new ReviewItemCheckDialog();
    dialog->setData(1);
    dialog->show();
}

void CreateReviewPage::handleDeleteReviewerItem(int id) {
    if (reviewerItems.size() <= 1) return;

    if (reviewerItems.contains(id)) {
        auto* item = reviewerItems.take(id);
        if (item) {
            reviewerItemLayout->removeWidget(item);
            item->deleteLater();
        }
    }

    reviewerItemContainer->adjustSize();
}

void CreateReviewPage::handleDeleteRequestItem(int id) {
    if (requestItems.size() <= 1) return;

    if (requestItems.contains(id)) {
        auto* item = requestItems.take(id);
        if (item) {
            requestItemLayout->removeWidget(item);
            item->deleteLater();
        }
    }

    requestItemContainer->adjustSize();
}

void CreateReviewPage::clickedAddChecklistButton() {
    ChecklistDialog* dialog = new ChecklistDialog(this);
    dialog->show();
}

void CreateReviewPage::clickedAddStandardButton() {
    StandardCreateDialog* dialog = new StandardCreateDialog(this);
    dialog->show();
}

void CreateReviewPage::clickedBackButton() {
    emit requestBack(0);
}

void CreateReviewPage::clickedCancelButton() {
    emit clickedBackButton();
}

void CreateReviewPage::clickedSaveButton() {
    ReviewData data;
    QDate date = dueDateEdit->date();

    data.name = reviewNameEdit->text();
    data.dueDate = date.toString("MMM d, yyyy");
    data.reviewLeader = reviewLeaderSelect->currentText();
    data.primaryReviewer = primaryReviewerSelect->currentText();
    data.changeRequests = 0;
    data.status = 0;

    // Review items
    for (int i = 0; i < reviewItemLayout->count(); ++i) {
        QWidget* widget = reviewItemLayout->itemAt(i)->widget();
        ReviewItem* item = qobject_cast<ReviewItem*>(widget);
        const QString& newReviewId = ReviewsController::generateUniqueId();
        if (item) data.reviewItems.insert(newReviewId, item->getData());
    }

    for (int i = 0; i < reviewerItemLayout->count(); ++i) {
        QWidget* widget = reviewerItemLayout->itemAt(i)->widget();
        ReviewerItem* item = qobject_cast<ReviewerItem*>(widget);
        if (item) data.reviewers.append(item->getData());
    }

    for (int i = 0; i < requestItemLayout->count(); ++i) {
        QWidget* widget = requestItemLayout->itemAt(i)->widget();
        RequestItem* item = qobject_cast<RequestItem*>(widget);
        if (item) data.requests.append(item->getData());
    }

    if (checklistSelect) {
        QStringList selected = checklistSelect->selectedItems();
        for (const QString& str : selected)
            data.checklists.append(ReviewsController::getChecklistId(str));
    }

    if (standardSelect) {
        QStringList selected = standardSelect->selectedItems();
        for (const QString& str : selected)
            data.standards.append(ReviewsController::getStandardId(str));
    }

    if (reviewId.isEmpty()) {
        data.id = ReviewsController::generateUniqueId();
        ReviewsController::addReview(data);
    } else {
        auto item = ReviewsController::getReviewById(reviewId);
        if (!item) return;

        data.id = item.value().id;
        ReviewsController::updateReview(reviewId, data);
    }

    emit clickedBackButton();
}
