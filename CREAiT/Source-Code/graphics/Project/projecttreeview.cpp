#include "projecttreeview.h"
#include <QDropEvent>
#include <QStandardItem>

ProjectTreeView::ProjectTreeView(QWidget *parent)
    : QTreeView{parent}
{
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDefaultDropAction(Qt::MoveAction);

    hoverTimer.setSingleShot(true);
    hoverTimer.setInterval(1500); // 1.5s delay
    connect(&hoverTimer, &QTimer::timeout, this, &ProjectTreeView::showPreview);
}

void ProjectTreeView::startDrag(Qt::DropActions supportedActions) {
    draggedIndexes = selectedIndexes();  // Save source indexes before drag
    qDebug() << "Drag started:" << draggedIndexes;
    QTreeView::startDrag(supportedActions);
}

void ProjectTreeView::dropEvent(QDropEvent *event) {
    QModelIndex targetIndex = indexAt(event->position().toPoint());
    if (!targetIndex.isValid()) targetIndex = rootIndex();

    QStandardItemModel *m = qobject_cast<QStandardItemModel*>(model());
    if (!m) return;

    QStandardItem *targetItem = m->itemFromIndex(targetIndex);
    if (!targetItem) return;

    bool isDirectory = targetItem->data(Qt::UserRole + 2).toString() == "Directory";
    QStandardItem *targetParentItem = nullptr;
    int insertRow = -1;

    QAbstractItemView::DropIndicatorPosition pos = dropIndicatorPosition();

    if (isDirectory && pos == QAbstractItemView::OnItem) {
        targetParentItem = targetItem;
        insertRow = targetItem->rowCount();
    } else {
        targetParentItem = targetItem->parent();
        if (!targetParentItem) targetParentItem = m->invisibleRootItem();

        if (pos == QAbstractItemView::AboveItem) {
            insertRow = targetItem->row();
        } else if (pos == QAbstractItemView::BelowItem) {
            insertRow = targetItem->row() + 1;
        } else {
            insertRow = targetItem->row();
        }
    }

    QStandardItem *srcItem = nullptr;
    for (const QModelIndex &src : draggedIndexes) {
        srcItem = m->itemFromIndex(src);
        if (!srcItem) continue;

        QString sourceRecordId = srcItem->data(Qt::UserRole).toString();
        QString targetRecordId = targetParentItem ? targetParentItem->data(Qt::UserRole).toString() : "-1";

        qDebug() << "Emitted recordsMoved for Record ID:" << sourceRecordId << " to Parent ID:" << targetRecordId << " at row:" << insertRow;
    }


    if (model()->dropMimeData(event->mimeData(), Qt::MoveAction, insertRow, -1, targetParentItem ? targetParentItem->index() : QModelIndex())) {
        event->acceptProposedAction();

        emit recordMoved(srcItem, targetParentItem, insertRow);
    } else {
        event->ignore();
    }
}

void ProjectTreeView::mouseReleaseEvent(QMouseEvent *event) {
    QModelIndex index = indexAt(event->pos());

    if (!index.isValid()) {
        QTreeView::mouseReleaseEvent(event);
        return;
    }

    if (!this->selectedIndexes().contains(index)) {
        QTreeView::mouseReleaseEvent(event);
        return;
    }

    QRect optionRect = visualRect(index);

    if (index.data(Qt::UserRole+2).toString() == "Directory") {
        QRect btnMenuRect(optionRect.right() - 56, optionRect.top() + 3, 22, 22);
        QRect btnPlusRect(optionRect.right() - 28, optionRect.top() + 3, 22, 22);

        if (btnPlusRect.contains(event->pos())) {
            emit actionPlusButtonClicked(index);
            return;
        } else if (btnMenuRect.contains(event->pos())) {
            emit actionMoreButtonClicked(index);
            return;
        }
    } else {
        QRect btnMenuRect(optionRect.right() - 28, optionRect.top() + 3, 22, 22);

        if (btnMenuRect.contains(event->pos())) {
            emit actionMoreButtonClicked(index);
            return;
        }
    }

    QTreeView::mouseReleaseEvent(event);
}

void ProjectTreeView::mouseMoveEvent(QMouseEvent *event) {
    QModelIndex index = indexAt(event->pos());

    if (index != hoveredIndex) {
        hoveredIndex = index;
        hoverGlobalPos = viewport()->mapToGlobal(event->pos());
        hoverTimer.start();
        qDebug() << "Item Move Hide";
        RecordSummaryPopover::instance()->hideIfNotHovered();
    } else {
        hoverGlobalPos = viewport()->mapToGlobal(event->pos());
    }

    QTreeView::mouseMoveEvent(event);
}

void ProjectTreeView::leaveEvent(QEvent *event) {
    QTreeView::leaveEvent(event);
    hoverTimer.stop();
    // qDebug() << "Item Leave";
    // RecordSummaryPopover::instance()->hideIfNotHovered();
}

void ProjectTreeView::showPreview() {
    if (!hoveredIndex.isValid())
        return;

    QString recordID = hoveredIndex.data(Qt::UserRole).toString();

    auto *popover = RecordSummaryPopover::instance();
    popover->loadRecord(recordID);
    popover->showAt(QCursor::pos() - QPoint(20, 20));
}
