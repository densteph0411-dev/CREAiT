#include "recorddatapresenter.h"
#include "../../dataObjects/dataDictionary.h"
#include "../../dataObjects/records.h"

RecordDataPresenter::RecordDataPresenter() {}

RecordDataPresenter& RecordDataPresenter::instance()
{
    static RecordDataPresenter _instance;
    return _instance;
}

bool RecordDataPresenter::loadRecord(QString recID)
{
    RecordDataPresenter& presenter = RecordDataPresenter::instance();

    std::string recordID = recID.toStdString();
    Data::setDataValue("ID", "record", recordID);
    dataRecord::getRecord();

    presenter.ID = QString::fromStdString(recordID);
    presenter.m_title = QString::fromStdString(Data::getDataValue("title", "record"));
    presenter.m_type = QString::fromStdString(Data::getDataValue("type", "record"));
    presenter.m_parent = QString::fromStdString(Data::getDataValue("parent", "record"));
    presenter.m_lastModifiedBy = QString::fromStdString(Data::getDataValue("lastModifiedBy", "record"));
    presenter.m_lastModifiedOn = QString::fromStdString(Data::getDataValue("lastModifiedOn", "record"));
    presenter.m_version = QString::fromStdString(Data::getDataValue("version", "record"));
    presenter.m_deleted = QString::fromStdString(Data::getDataValue("deleted", "record"));
    presenter.m_status = QString::fromStdString(Data::getDataValue("status", "record"));
    presenter.m_description = QString::fromStdString(Data::getDataValue("description", "record"));
    presenter.m_treeViewIndex = QString::fromStdString(Data::getDataValue("treeViewIndex", "record"));

    presenter.m_extraFields.clear();
    std::vector<std::string> keys = Data::getDictionaryKeysByType("record");
    for (const auto& key : keys) {
        if (key == "ID" || key == "title" || key == "type" || key == "description" ||
            key == "lastModifiedBy" || key == "lastModifiedOn" || key == "status" ||
            key == "version" || key == "parent" || key == "fileName" || key == "treeViewIndex"  ||
            key == "deleted") {
            continue;
        }

        std::string value = Data::getDataValue(key, "record");
        presenter.setExtraField(QString::fromStdString(key), QString::fromStdString(value));
    }

    presenter.m_links.clear();
    for (int i=0; i<dataRecord::linkName.size(); i++) {
        LinkRecord link;
        link.linkID = QString::fromStdString(dataRecord::linkName[i]);
        link.title = QString::fromStdString(dataRecord::linkTitle[i]);
        link.relationship = QString::fromStdString(dataRecord::linkDirection[i]);
        link.type = QString::fromStdString(dataRecord::linkType[i]);
        presenter.addLink(link);
    }

    presenter.m_versions.clear();
    for (int i=0; i<dataRecord::versionNumber.size(); i++) {
        VersionHistory version;
        version.number = QString::fromStdString(dataRecord::versionNumber[i]);
        version.description = QString::fromStdString(dataRecord::versionDescription[i]);
        version.date = QString::fromStdString(dataRecord::versionDate[i]);

        presenter.m_versions.append(version);
    }
    return true;
}

QString RecordDataPresenter::recordID() const
{
    return ID;
}

QString RecordDataPresenter::title() const
{
    return m_title;
}

QString RecordDataPresenter::type() const
{
    return m_type;
}

QString RecordDataPresenter::parent() const
{
    return m_parent;
}

QString RecordDataPresenter::lastModifiedBy() const
{
    return m_lastModifiedBy;
}

QString RecordDataPresenter::lastModifiedOn() const
{
    return m_lastModifiedOn;
}

QString RecordDataPresenter::version() const
{
    return m_version;
}

QString RecordDataPresenter::status() const
{
    return m_status;
}

QString RecordDataPresenter::deleted() const
{
    return m_deleted;
}

QString RecordDataPresenter::description() const
{
    return m_description;
}

QString RecordDataPresenter::treeViewIndex() const
{
    return m_treeViewIndex;
}

void RecordDataPresenter::setTitle(const QString& title)
{
    m_title = title;
}

void RecordDataPresenter::setType(const QString& type)
{
    m_type = type;
}

void RecordDataPresenter::setParent(const QString& parent)
{
    m_parent = parent;
}

void RecordDataPresenter::setDeleted(const QString& deleted)
{
    m_deleted = deleted;
}

void RecordDataPresenter::setLastModifiedBy(const QString& lastModifiedBy)
{
    m_lastModifiedBy = lastModifiedBy;
}

void RecordDataPresenter::setLastModifiedOn(const QString& lastModifiedOn)
{
    m_lastModifiedOn = lastModifiedOn;
}

void RecordDataPresenter::setStatus(const QString& status)
{
    m_status = status;
}

void RecordDataPresenter::setVersion(const QString& version)
{
    m_version = version;
}

void RecordDataPresenter::setDescription(const QString& description)
{
    m_description = description;
}

void RecordDataPresenter::setTreeViewIndex(const QString& treeIndex)
{

}

void RecordDataPresenter::updateRecord()
{
    Data::setDataValue("ID", "record", ID.toStdString());
    dataRecord::getRecord();

    Data::setDataValue("title", "record", m_title.toStdString());
    Data::setDataValue("type", "record", m_type.toStdString());
    Data::setDataValue("parent", "record", m_parent.toStdString());
    Data::setDataValue("lastModifiedBy", "record", m_lastModifiedBy.toStdString());
    Data::setDataValue("lastModifiedOn", "record", m_lastModifiedOn.toStdString());
    Data::setDataValue("version", "record", m_version.toStdString());
    Data::setDataValue("status", "record", m_status.toStdString());
    Data::setDataValue("description", "record", m_description.toStdString());
    Data::setDataValue("deleted", "record", m_deleted.toStdString());
}

QList<QString> RecordDataPresenter::extraFieldKeys() const
{
    return m_extraFields.keys();
}

QVariant RecordDataPresenter::extraField(const QString& key) const
{
    return m_extraFields.value(key, QVariant());
}

bool RecordDataPresenter::hasExtraField(const QString& key) const
{
    return m_extraFields.contains(key);
}

void RecordDataPresenter::setExtraField(const QString& key, const QVariant& value)
{
    m_extraFields.insert(key, value);
}

const QList<LinkRecord>& RecordDataPresenter::links() const {
    return m_links;
}

void RecordDataPresenter::setLinks(const QList<LinkRecord>& links) {
    m_links = links;
}

void RecordDataPresenter::addLink(const LinkRecord& link, bool persist) {
    m_links.append(link);

    if (persist) {
        dataRecord::addLink(link.linkID.toStdString(), link.relationship == "Up", link.type.toStdString());
    }
}

const QList<VersionHistory>& RecordDataPresenter::versions() const {
    return m_versions;
}

QList<RecordType> RecordDataPresenter::getRecordTypeList() {
    QList<RecordType> typeList;
    // Get all of the recordTypes and innerText
    std::string title;
    std::vector<std::string> allRecordTypes;
    allRecordTypes = Data::getDictionaryKeysByType("recordType");

    // Add record Types as folders to the project
    // Test of recordTypes is empty
    if ((!allRecordTypes.empty())) {
        // Add directories for each record type
        for (const auto& key : allRecordTypes) {

            // Get the innert text as the title
            title = Data::getDataValue(key, "recordType");

            RecordType recordType;
            recordType.key = QString::fromStdString(key);
            recordType.type = QString::fromStdString(title);

            typeList.append(recordType);
        }
    }

    return typeList;
}

QList<LinkType> RecordDataPresenter::getLinkTypeList() {
    QList<LinkType> typeList;
    // Get all of the recordTypes and innerText
    std::string title;
    std::vector<std::string> listTypes;
    listTypes = Data::getDictionaryKeysByType("linkType");

    // Add record Types as folders to the project
    // Test of recordTypes is empty
    if ((!listTypes.empty())) {
        // Add directories for each record type
        for (const auto& key : listTypes) {

            // Get the innert text as the title
            title = Data::getDataValue(key, "recordType");

            LinkType linkType;
            linkType.key = QString::fromStdString(key);
            linkType.type = QString::fromStdString(title);

            typeList.append(linkType);
        }
    }

    return typeList;
}
