#ifndef LINKRECORDTABLEMODEL_H
#define LINKRECORDTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>

struct LinkRecordItem {
    bool checked = false;
    QString id;
    QString title;
    QString type;
};

class LinkRecordTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit LinkRecordTableModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void setFilter(const QString &text);
    void resetData(const QVector<LinkRecordItem> &data);

    QVector<LinkRecordItem> checkedItems() const;
private:
    QVector<LinkRecordItem> allItems;
    QVector<LinkRecordItem> filteredItems;
};

#endif // LINKRECORDTABLEMODEL_H
