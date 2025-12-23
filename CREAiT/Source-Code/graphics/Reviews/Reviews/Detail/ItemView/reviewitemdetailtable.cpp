#include "reviewitemdetailtable.h"
#include <reviewscontroller.h>

ReviewItemDetailTable::ReviewItemDetailTable(QWidget *parent)
    : QWidget(parent)
{
    initializeWidget();
}

ReviewItemDetailTable::~ReviewItemDetailTable()
{

}

void ReviewItemDetailTable::initializeWidget() {
    setupUi(this);

    endVersionRow->setLastRow();
}

void ReviewItemDetailTable::setData(const QString& parentId, const QString& reviewItemId) {
    auto item = ReviewsController::getReviewItem(parentId, reviewItemId);
    if (!item) return;

    const ReviewItemData& data = item.value();
    startVersionRow->setData(data.startVersionData);
    endVersionRow->setData(data.endVersionData);
}
