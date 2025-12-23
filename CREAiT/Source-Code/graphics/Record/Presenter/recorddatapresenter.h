#ifndef RECORDDATAPRESENTER_H
#define RECORDDATAPRESENTER_H

#include <QString>
#include <QHash>
#include <QVariant>

struct LinkRecord {
    QString linkID;
    QString title;
    QString relationship;
    QString type;
};

struct VersionHistory {
    QString number;
    QString description;
    QString date;
};

struct RecordType {
    QString key;
    QString type;
};

struct LinkType {
    QString key;
    QString type;
};


class RecordDataPresenter
{
public:
    static RecordDataPresenter& instance();  // singleton accessor

    // Prevent copying
    RecordDataPresenter(const RecordDataPresenter&) = delete;
    RecordDataPresenter& operator=(const RecordDataPresenter&) = delete;

    QString recordID() const;
    QString title() const;
    QString type() const;
    QString parent() const;
    QString lastModifiedBy() const;
    QString lastModifiedOn() const;
    QString version() const;
    QString status() const;
    QString deleted() const;
    QString description() const;
    QString treeViewIndex() const;

    void setTitle(const QString& title);
    void setType(const QString& type);
    void setParent(const QString& parent);
    void setLastModifiedBy(const QString& lastModifiedBy);
    void setLastModifiedOn(const QString& lastModifiedOn);
    void setVersion(const QString& version);
    void setStatus(const QString& status);
    void setDescription(const QString& description);
    void setTreeViewIndex(const QString& treeIndex);
    void setDeleted(const QString& deleted);
    void updateRecord();

    // Extra field accessors
    QVariant extraField(const QString& key) const;
    bool hasExtraField(const QString& key) const;
    void setExtraField(const QString& key, const QVariant& value);
    QList<QString> extraFieldKeys() const;

    const QList<LinkRecord>& links() const;
    void setLinks(const QList<LinkRecord>& links);
    void addLink(const LinkRecord& link, bool persist = false);

    const QList<VersionHistory>& versions() const;

    static bool loadRecord(QString recID);

    static QList<RecordType> getRecordTypeList();
    static QList<LinkType> getLinkTypeList();
private:
    RecordDataPresenter();

    QString ID;
    QString m_title;
    QString m_type;
    QString m_parent;
    QString m_lastModifiedBy;
    QString m_lastModifiedOn;
    QString m_version;
    QString m_status;
    QString m_deleted;
    QString m_description;
    QString m_treeViewIndex;

    QHash<QString, QVariant> m_extraFields;
    QList<LinkRecord> m_links;
    QList<VersionHistory> m_versions;
};

#endif // RECORDDATAPRESENTER_H
