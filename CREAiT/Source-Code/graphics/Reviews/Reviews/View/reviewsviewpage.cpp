#include "reviewsviewpage.h"

ReviewsViewPage::ReviewsViewPage(QWidget *parent)
    : QWidget(parent)
{
    initializeWidgets();
}

ReviewsViewPage::~ReviewsViewPage()
{

}

void ReviewsViewPage::initializeWidgets() {
    setupUi(this);

    deleteDialog = new DeleteDialog();

    reviewModel = new QStandardItemModel(this);
    reviewModel->setColumnCount(5);
    reviewModel->setHorizontalHeaderLabels({"Review ID", "Review Name", "Review Leader", "Primary Reviewer", "Change Requests", "Review Status", "Options"});

    reviewTableView->setModel(reviewModel);
    reviewTableView->setShowGrid(false);
    reviewTableView->setAlternatingRowColors(false);
    reviewTableView->setTextElideMode(Qt::ElideNone);
    reviewTableView->setWordWrap(true);
    reviewTableView->setSelectionMode(QAbstractItemView::NoSelection);
    reviewTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    reviewTableView->setFocusPolicy(Qt::NoFocus);
    reviewTableView->setIconSize(QSize(24, 24));
    reviewTableView->horizontalHeader()->setFixedHeight(46);
    reviewTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    reviewTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    reviewTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    reviewTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    reviewTableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    reviewTableView->horizontalHeader()->setStretchLastSection(true);
    reviewTableView->verticalHeader()->setVisible(false);
    reviewTableView->verticalHeader()->setDefaultSectionSize(60);
    reviewTableView->resizeColumnsToContents();

    connect(createButton, &QPushButton::clicked, this, &ReviewsViewPage::clickedAddReviewButton);
    connect(createButton, &QPushButton::clicked, this, &ReviewsViewPage::clickedAddReviewButton);
    connect(deleteDialog, &DeleteDialog::requestDelete, this, &ReviewsViewPage::handleReviewDelete);

    connect(reviewTableView, &QTableView::doubleClicked, this, &ReviewsViewPage::doubleClickedTableItem);
}

void ReviewsViewPage::refresh() {
    reviewData = ReviewsController::getReviews();
    reviewModel->removeRows(0, reviewModel->rowCount());

    int currentRow = 0;
    for (auto id: reviewData.keys()) {
        QList<QStandardItem*> rowItems;
        QStandardItem* idItem = new QStandardItem(reviewData[id].id);
        idItem->setData(id, Qt::UserRole);
        rowItems << idItem
            << new QStandardItem(reviewData[id].name)
            << new QStandardItem(reviewData[id].reviewLeader)
            << new QStandardItem(reviewData[id].primaryReviewer)
            << new QStandardItem(QString::number(reviewData[id].changeRequests))
            << new QStandardItem()
            << new QStandardItem();

        reviewModel->appendRow(rowItems);

        QPushButton* statusIcon = new QPushButton();
        statusIcon->setProperty("class", reviewStatusLabels.value(reviewData[id].status));
        statusIcon->setText(reviewStatusLabels.value(reviewData[id].status));
        statusIcon->setFixedSize(105, 28);
        reviewTableView->setIndexWidget(reviewModel->index(currentRow, 5), statusIcon);

        QWidget *actionWidget = new QWidget();
        QPushButton *viewBtn = new QPushButton();
        QPushButton *editBtn = new QPushButton();
        QPushButton *deleteBtn = new QPushButton();

        viewBtn->setIcon(QIcon(":/images/Common/TableView/view.svg"));
        editBtn->setIcon(QIcon(":/images/Common/TableView/edit.svg"));
        deleteBtn->setIcon(QIcon(":/images/Common/TableView/delete.svg"));
        viewBtn->setToolTip("View");
        editBtn->setToolTip("Edit");
        deleteBtn->setToolTip("Delete");

        viewBtn->setProperty("reviewId", id);
        editBtn->setProperty("reviewId", id);
        deleteBtn->setProperty("reviewId", id);

        connect(viewBtn, &QPushButton::clicked, this, &ReviewsViewPage::handleViewClicked);
        connect(editBtn, &QPushButton::clicked, this, &ReviewsViewPage::handleEditClicked);
        connect(deleteBtn, &QPushButton::clicked, this, &ReviewsViewPage::handleDeleteClicked);

        QHBoxLayout *layout = new QHBoxLayout(actionWidget);
        layout->addWidget(viewBtn);
        layout->addWidget(editBtn);
        layout->addWidget(deleteBtn);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(5);
        layout->addStretch();
        actionWidget->setLayout(layout);

        reviewTableView->setIndexWidget(reviewModel->index(currentRow, 6), actionWidget);
        currentRow++;
    }
}

void ReviewsViewPage::setReviews() {

}

void ReviewsViewPage::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    refresh();
}

void ReviewsViewPage::clickedAddReviewButton() {
    emit requestCreate(4, "");
}

void ReviewsViewPage::doubleClickedTableItem(const QModelIndex& index) {
    if (!index.isValid()) return;

    QModelIndex idIndex = index.sibling(index.row(), 0);
    QString reviewId = idIndex.data(Qt::UserRole).toString();

    emit requestDetail(5, reviewId);
}

void ReviewsViewPage::handleViewClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    QString reviewId = btn->property("reviewId").toString();

    emit requestDetail(5, reviewId);
}

void ReviewsViewPage::handleEditClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    QString reviewId = btn->property("reviewId").toString();

    emit requestCreate(4, reviewId);
}

void ReviewsViewPage::handleDeleteClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    deleteId = btn->property("reviewId").toString();
    deleteDialog->show();
}

void ReviewsViewPage::handleReviewDelete() {
    if (deleteId.isEmpty()) return;

    ReviewsController::deleteReviewById(deleteId);
    refresh();
    deleteId = "";
}
