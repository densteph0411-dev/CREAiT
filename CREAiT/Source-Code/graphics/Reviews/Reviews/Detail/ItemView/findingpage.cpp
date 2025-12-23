#include "findingpage.h"
#include "reviewactiondialog.h"


FindingPage::FindingPage(QWidget *parent)
    : QWidget(parent)
{
    initializeWidgets();
}

FindingPage::~FindingPage() {

}

void FindingPage::initializeWidgets() {
    setupUi(this);

    assigneeSelect->addItems(tempReviewers);
    assigneeSelect->setEditable(false);

    verifierSelect->addItems(tempReviewers);
    verifierSelect->setEditable(false);

    actionMenu = new QMenu();
    actionMenu->addAction("Implemented", this, &::FindingPage::requestImplement);
    actionMenu->addAction("Verified", this, &::FindingPage::requestVerification);
    actionMenu->addAction("Rejected", this, &::FindingPage::requestReject);

    actionMenu->setStyleSheet(R"(
        QMenu {
            font-family: Open Sans;
            font-weight: 500;
            font-size: 14px;
            line-height: 22px;
            letter-spacing: 0%;
            color: #1A1A1A;
            background-color: white;
            border: 1px solid #ccc;
            border-radius: 8px;
            padding: 12px;
        }
        QMenu::item {
            padding: 6px 12px;
            border-radius: 6px;
        }
        QMenu::item:selected {
            background-color: #f0f0f0;
        }
    )");

    findingWidget->Header->hide();
    commentPanel->clear(1);

    connect(backButton, &QPushButton::clicked, this, &FindingPage::clickedBackButton);
    connect(menuButton, &QPushButton::clicked, this, &FindingPage::clickedMenuButton);
}

void FindingPage::setData(const QString& parentId, const QString& itemId, const QString& subItemId) {
    reviewId = parentId;
    reviewItemId = itemId;
    findingId = subItemId;

    auto item = ReviewsController::getFindingData(reviewId, reviewItemId, findingId);
    if (!item) return;

    const FindingData& data = item.value();

    findingIDValueLabel->setText(data.id);
    assigneeSelect->setCurrentText(data.assignee);
    verifierSelect->setCurrentText(data.verifier);
    findingWidget->setFindingData(reviewId, reviewItemId, data);
    reviewItemDetailTable->setData(reviewId, reviewItemId);
    commentPanel->setCommentData(reviewId, reviewItemId, findingId);
}

void FindingPage::clickedBackButton() {
    emit requestBack(6);
}

void FindingPage::clickedMenuButton() {
    QPoint globalPos = menuButton->mapToGlobal(QPoint(0, menuButton->height()));
    actionMenu->exec(globalPos);
}

void FindingPage::requestImplement() {
    menuButton->setText("Implemented");

    ReviewActionDialog* dialog = new ReviewActionDialog();
    dialog->setData(-1, 0, 0, "", "");
    dialog->show();
}

void FindingPage::requestVerification() {
    menuButton->setText("Verified      ");
    ReviewActionDialog* dialog = new ReviewActionDialog();
    dialog->setData(-1, 2, 0, "", "");
    dialog->show();
}

void FindingPage::requestReject() {
    menuButton->setText("Rejected     ");
    ReviewActionDialog* dialog = new ReviewActionDialog();
    dialog->setData(-1, 3, 0, "", "");
    dialog->show();
}
