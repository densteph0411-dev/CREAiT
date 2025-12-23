#ifndef RECORDTREEITEM_H
#define RECORDTREEITEM_H

#include <QStandardItem>

class RecordTreeItem : public QStandardItem
{
public:
    RecordTreeItem(const QString &text, const QString& type);
    QString recordType() const { return m_type; }

    void setRecordType(QString type) {
        m_type = type;
        updateIcon();
    }

    bool isDeleted();
    void setDeleted(bool isDeleted);
private:
    QString m_type;
    bool m_deleted;
    void updateIcon();
};

#endif // RECORDTREEITEM_H
