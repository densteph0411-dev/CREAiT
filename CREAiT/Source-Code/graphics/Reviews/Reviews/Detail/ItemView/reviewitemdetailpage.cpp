#include "reviewitemdetailpage.h"
#include <reviewscontroller.h>

ReviewItemDetailPage::ReviewItemDetailPage(QWidget *parent)
    : QWidget(parent)
{
    initializeWidgets();
}

ReviewItemDetailPage::~ReviewItemDetailPage() {

}

void ReviewItemDetailPage::initializeWidgets() {
    setupUi(this);

    containerLayout->setAlignment(Qt::AlignTop);
    saveButton->hide();

    startVersion->icon->setIcon(QIcon(":images/Reviews/version.svg"));
    startVersion->titleLabel->setText("Start Version");


    endVersion->icon->setIcon(QIcon(":images/Reviews/version.svg"));
    endVersion->titleLabel->setText("End Version");


    findingPanel->clear(0);

    connect(backButton, &QPushButton::clicked, this, [this]() {
        emit requestBack(0);
    });

    connect(findingPanel, &FindingCommentPanel::requestFinding, this, &ReviewItemDetailPage::requestFinding);
}

void ReviewItemDetailPage::setData(const QString& parentId, const QString& itemId) {
    reviewId = parentId;
    reviewItemId = itemId;

    auto item = ReviewsController::getReviewItem(reviewId, reviewItemId);
    if (!item) return;

    const ReviewItemData& data = item.value();

    backButton->setText(data.item);
    startVersion->description->setText(data.startVersionData.id);
    endVersion->description->setText(data.endVersionData.id);
    reviewItemDetailTable->setData(parentId, itemId);
    findingPanel->setFindingData(reviewId, reviewItemId);
}
