#include "projecttreemodel.h"

ProjectTreeModel::ProjectTreeModel(QObject *parent)
    : QStandardItemModel{parent}
{}

// bool ProjectTreeModel::canDropMimeData(const QMimeData *data, Qt::DropAction action,
//                                   int row, int column, const QModelIndex &parent) const {
//     // First check base class rules
//     if (!QStandardItemModel::canDropMimeData(data, action, row, column, parent))
//         return false;

//     // Get the target parent item
//     RecordTreeItem *parentItem = (RecordTreeItem*) this->itemFromIndex(parent);
//     if (!parentItem)
//         return false; // Drop target is invalid

//     // Check if parentItem is a directory type
//     bool isDirectory = parentItem->data(Qt::UserRole + 2).toString() == "Directory"; // Assuming 1=DirectoryType

//     if (!isDirectory) {
//         qDebug() << "Drop blocked: target is not a directory.";
//         return false;
//     }

//     return true; // Allow drop
// }
