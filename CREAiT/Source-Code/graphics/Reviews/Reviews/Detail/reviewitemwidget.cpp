#include "reviewitemwidget.h"
#include "findingviewdialog.h"

ReviewItemWidget::ReviewItemWidget(QWidget *parent)
    : QWidget(parent)
{
    initializeWidgets();
}

ReviewItemWidget::~ReviewItemWidget()
{

}

void ReviewItemWidget::initializeWidgets() {
    setupUi(this);

    connect(showFindingButton, &QPushButton::clicked, this, &ReviewItemWidget::clickedShowFindingButton);
    connect(viewDetailButton, &QPushButton::clicked, this, &ReviewItemWidget::clickedViewDetailButton);
}

void ReviewItemWidget::setData(const QString& id, const ReviewItemData& data) {
    reviewId = id;
    reviewItemId = data.id;
    currentData = data;

    mainLabel->setText(data.item);
    startVerValueLabel->setText(data.startVersionData.id);
    endVerValueLabel->setText(data.endVersionData.id);
}

void ReviewItemWidget::clickedShowFindingButton() {
    FindingViewDialog* dialog = new FindingViewDialog(this);
    dialog->setData(reviewId, reviewItemId, currentData.findings);
    dialog->show();
}

void ReviewItemWidget::clickedViewDetailButton() {
    emit requestDetail(reviewId, reviewItemId);
}
