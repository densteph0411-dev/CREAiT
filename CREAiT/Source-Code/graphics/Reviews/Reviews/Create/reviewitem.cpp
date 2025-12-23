#include "reviewitem.h"
#include "recordtreeitem.h"
#include <Project/Presenter/projectdatapresenter.h>
#include <queue>
#include <reviewscontroller.h>

ReviewItem::ReviewItem(int id, QWidget *parent)
    : QWidget(parent), index(id)
{
    initializeWidgets();
}

ReviewItem::~ReviewItem() {

}

void ReviewItem::initializeWidgets() {
    setupUi(this);
    refresh();
    connect(deleteButton, &QPushButton::clicked, this, &ReviewItem::clickedDeleteButton);
}

ReviewItemData ReviewItem::getData() const {
    ReviewItemData data;

    data.item = reviewItemSelect->selectedDirID();
    data.startVersionData.id = startVersionSelect->currentText();
    data.endVersionData.id = endVersionSelect->currentText();

    return data;
}

void ReviewItem::setData(const ReviewItemData& data) {
    refresh();

    // reviewItemSelect.set
    reviewItemSelect->setSelectedDirID(data.item);
    startVersionSelect->setCurrentText(data.startVersionData.id);
    endVersionSelect->setCurrentText(data.endVersionData.id);
}

void ReviewItem::refresh() {
    startVersionSelect->clear();
    endVersionSelect->clear();

    std::map<std::string, RecordTreeItem*> recordItemMap;
    auto *model = new QStandardItemModel(this);

    ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();
    QList<Record> recordList = projInstance.fetchAllRecordList();

    // Step 1: Load all records into a map
    for (const auto &record : recordList) {
        RecordTreeItem *item = NULL;

        item = new RecordTreeItem(record.title, record.type);
        item->setData(record.id, Qt::UserRole);
        item->setData(record.parent, Qt::UserRole + 1);
        item->setData(record.type, Qt::UserRole + 2);
        item->setData(record.treeViewIndex, Qt::UserRole + 3);

        if (record.deleted == "true") {
            item->setData(true, Qt::UserRole + 4);
        } else {
            item->setData(false, Qt::UserRole + 4);
        }

        recordItemMap[record.id.toStdString()] = item;
    }

    // Step 2: Build tree hierarchy after all items are created

    std::string rootID = "PROJ";
    std::queue<std::string> q;
    q.push(rootID);

    while (!q.empty()) {
        std::string currentID = q.front();

        RecordTreeItem *curItem = recordItemMap[currentID];
        std::string parentID = curItem->data(Qt::UserRole + 1).toString().toStdString();
        std::string type = curItem->data(Qt::UserRole + 2).toString().toStdString();
        bool deleted = curItem->data(Qt::UserRole + 4).toBool();
        if (!deleted && type=="Directory") {
            RecordTreeItem *parentItem = nullptr;
            if (!parentID.empty() && recordItemMap.count(parentID)) {
                parentItem = recordItemMap[parentID];
            }

            if (!parentItem || parentID == "PROJ")
                parentItem = (RecordTreeItem*)model->invisibleRootItem();

            parentItem->appendRow(curItem);
        }

        q.pop();

        QList<Record> childList = projInstance.fetchChildRecordList(QString::fromStdString(currentID));
        // Push children to queue
        for (int i=0; i<childList.count(); i++) {
            q.push(childList[i].id.toStdString());
        }
    }

    reviewItemSelect->setModel(model);
    startVersionSelect->addItems(tempVerions);
    endVersionSelect->addItems(tempVerions);
}

void ReviewItem::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    // refresh();
}

void ReviewItem::clickedDeleteButton() {
    emit requestDelete(index);
}
