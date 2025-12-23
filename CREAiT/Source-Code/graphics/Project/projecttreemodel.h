#ifndef PROJECTTREEMODEL_H
#define PROJECTTREEMODEL_H

#include <QSortFilterProxyModel>
#include <QStandardItemModel>

class ProjectTreeModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit ProjectTreeModel(QObject *parent = nullptr);

    // // Override to restrict drop only to directory items
    // bool canDropMimeData(const QMimeData *data, Qt::DropAction action,
    //                      int row, int column, const QModelIndex &parent) const override;
};

#endif // PROJECTTREEMODEL_H
