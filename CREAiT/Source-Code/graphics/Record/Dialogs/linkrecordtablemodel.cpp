#include "linkrecordtablemodel.h"

LinkRecordTableModel::LinkRecordTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

QVariant LinkRecordTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return "";  // Checkbox column
        case 1: return "ID";
        case 2: return "Title";
        case 3: return "Type";
        }
    }
    return QVariant();
}

int LinkRecordTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return filteredItems.size();
}

int LinkRecordTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 4; // Checkbox + 3 columns
}

QVariant LinkRecordTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();
    const auto &item = filteredItems.at(index.row());

    if (index.column() == 0) {
        if (role == Qt::CheckStateRole) return item.checked ? Qt::Checked : Qt::Unchecked;
    }

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 1: return item.id;
        case 2: return item.title;
        case 3: return item.type;
        }
    }
    return {};
}

bool LinkRecordTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) return false;

    if (index.column() == 0 && role == Qt::CheckStateRole) {
        filteredItems[index.row()].checked = value.toInt() == Qt::Checked;
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags LinkRecordTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (index.column() == 0)
        flags |= Qt::ItemIsUserCheckable;
    return flags;
}

void LinkRecordTableModel::resetData(const QVector<LinkRecordItem> &data) {
    beginResetModel();
    allItems = data;
    filteredItems = data;
    endResetModel();
}

void LinkRecordTableModel::setFilter(const QString &text) {
    beginResetModel();
    filteredItems.clear();
    for (const auto &item : allItems) {
        if (item.id.contains(text, Qt::CaseInsensitive) ||
            item.title.contains(text, Qt::CaseInsensitive) ||
            item.type.contains(text, Qt::CaseInsensitive)) {
            filteredItems.push_back(item);
        }
    }
    endResetModel();
}

QVector<LinkRecordItem> LinkRecordTableModel::checkedItems() const
{
    QVector<LinkRecordItem> result;
    for (const auto &item : filteredItems) {
        if (item.checked)
            result.append(item);
    }
    return result;
}
