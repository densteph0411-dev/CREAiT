#include "projectdatapresenter.h"
#include "dataDictionary.h"
#include "records.h"
#include <filesystem>

ProjectDataPresenter::ProjectDataPresenter() {}

ProjectDataPresenter& ProjectDataPresenter::instance()
{
    static ProjectDataPresenter _instance;
    return _instance;
}

QString ProjectDataPresenter::GetProjectName()
{
    std::string projectName = Data::getDataValue("projectName", "project");
    return QString::fromStdString(projectName);
}

void ProjectDataPresenter::SetProjectName(const QString& projectName) {
    Data::setDataValue("projectName", "project", projectName.toStdString());
}

void ProjectDataPresenter::loadProject(const QString& projectsPath, const QString& projectName) {
    if (!projectName.isEmpty())
        ProjectDataPresenter::SetProjectName(projectName);

    std::string name = Data::getDataValue("projectName", "project");
    std::string fileName = Data::getDataValue("fileNameOnly", "project");

    projectInfoRecord::load(projectsPath.toStdString() + "/" + fileName, name);
    m_projectPath = projectsPath;

    recordList.clear();

    std::vector<std::string> allRecords = projectInfoRecord::getAllRecords();

    // Step 1: Load all records into a map
    for (const auto &recordID : allRecords) {
        Data::setDataValue("ID", "record", recordID);
        dataRecord::getRecord();

        std::string title = Data::getDataValue("title", "record");
        std::string type = Data::getDataValue("type", "record");
        std::string parent = Data::getDataValue("parent", "record");
        std::string deleted = Data::getDataValue("deleted", "record");
        std::string status = Data::getDataValue("status", "record");
        std::string treeViewInex = Data::getDataValue("treeViewIndex", "record");

        Record item;
        item.id = QString::fromStdString(recordID);
        item.title = QString::fromStdString(title);
        item.type = QString::fromStdString(type);
        item.parent = QString::fromStdString(parent);
        item.deleted = QString::fromStdString(deleted);
        item.status = QString::fromStdString(status);
        item.treeViewIndex = QString::fromStdString(treeViewInex);

        recordList.push_back(item);
    }

    // Get all of the recordTypes and innerText
    std::string title;
    std::vector<std::string> allRecordTypes;
    allRecordTypes = Data::getDictionaryKeysByType("recordType");

    recordTypeList.clear();
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

            recordTypeList.append(recordType);
        }
    }
}

QString ProjectDataPresenter::getProjectPath() {
    return m_projectPath;
}

QList<Record> ProjectDataPresenter::fetchAllRecordList() {
    return recordList;
}

QList<Record> ProjectDataPresenter::fetchChildRecordList(QString parent) {
    QList<Record> childlist;
    QList<Record> sortlist;
    for (int i=0; i<recordList.count(); i++) {
        if (recordList[i].parent == parent) {
            childlist.push_back(recordList[i]);
        }
    }

    int childCount = childlist.count();
    QList<Record> pendingList;

    while(sortlist.count() < childCount) {
        if (pendingList.empty()) {
            pendingList.push_front(childlist.front());
            childlist.pop_front();
        }

        Record currentTop = pendingList.front();
        int aboveIndex = -1;
        for (int i=0; i<childlist.count(); i++) {
            if (currentTop.treeViewIndex == childlist[i].id) {
                aboveIndex = i;
                break;
            }
        }

        if (aboveIndex != -1) {
            pendingList.push_front(childlist[aboveIndex]);
            childlist.remove(aboveIndex);
        }

        Record currentBottom = pendingList.back();
        int belowIndex = -1;
        for (int i=0; i<childlist.count(); i++) {
            if (childlist[i].treeViewIndex == currentBottom.id) {
                belowIndex = i;
                break;
            }
        }

        if (belowIndex != -1) {
            pendingList.push_back(childlist[belowIndex]);
            childlist.remove(belowIndex);
        }

        if (aboveIndex == -1 && belowIndex == -1) {
            if (pendingList.front().treeViewIndex == "") {
                while (pendingList.count() > 0) {
                    sortlist.push_front(pendingList.back());
                    pendingList.pop_back();
                }
            } else {
                while (pendingList.count() > 0) {
                    sortlist.push_back(pendingList.front());
                    pendingList.pop_front();
                }
            }
        }
    }

    return sortlist;
}

QList<Record> ProjectDataPresenter::searchRecordsByKeyword(QString keyword) {
    QList<Record> recordList;
    std::vector<std::string> childRecords =  projectInfoRecord::getAllRecords();
    QString lowerKeyword = keyword.trimmed().toLower();

    // Step 1: Load all records into a map
    for (const auto &recordID : childRecords) {
        Data::setDataValue("ID", "record", recordID);
        dataRecord::getRecord();

        QString title = QString::fromStdString(Data::getDataValue("title", "record"));
        QString type = QString::fromStdString(Data::getDataValue("type", "record"));
        QString parent = QString::fromStdString(Data::getDataValue("parent", "record"));
        QString deleted = QString::fromStdString(Data::getDataValue("deleted", "record"));
        QString desc = QString::fromStdString(Data::getDataValue("description", "record"));

        if (deleted == "true") {
            continue;
        }

        if (title.toLower().contains(lowerKeyword) || desc.toLower().contains(lowerKeyword)) {
            Record rec;

            rec.id = QString::fromStdString(recordID);
            rec.title = title;
            rec.type = type;
            rec.parent = parent;
            rec.description = desc;
            recordList.append(rec);
        }
    }

    return recordList;
}

QList<RecordType>* ProjectDataPresenter::getRecordTypeList()
{
    return &this->recordTypeList;
}

QString ProjectDataPresenter::getRecordIconPath(Record record)
{
    ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();
    QString iconDirPath = projInstance.getProjectPath() + "/" + projInstance.GetProjectName() + "/" + "images";

    QString typeKey = "";
    QList<RecordType> typeList = RecordDataPresenter::getRecordTypeList();
    for (const auto &type: typeList) {
        if (type.type == record.type) {
            typeKey = type.key;
        }
    }

    QString iconFilePath = iconDirPath + "/" + typeKey + ".svg";

    if (!std::filesystem::exists(iconFilePath.toStdString())) {
        if (record.type == "Directory")
            iconFilePath = ":/images/Project/TypeIcons/DIR.svg";
        else if (record.type == "System Requirement")
            iconFilePath = ":/images/Project/TypeIcons/SyREQ.svg";
        else if (record.type == "System Test Case")
            iconFilePath = ":/images/Project/TypeIcons/SyTC.svg";
        else if (record.type == "System Test")
            iconFilePath = ":/images/Project/TypeIcons/SyTest.svg";
        else if (record.type == "Software Requirement")
            iconFilePath = ":/images/Project/TypeIcons/SwREQ.svg";
        else if (record.type == "Software Test Case")
            iconFilePath = ":/images/Project/TypeIcons/SwTC.svg";
        else if (record.type == "Code")
            iconFilePath = ":/images/Project/TypeIcons/Code.svg";
        else
            iconFilePath = ":/images/Project/TypeIcons/Extra.svg";
    }

    return iconFilePath;
}

QString ProjectDataPresenter::createRecord(const QString recordType,
                                          const QString title,
                                          const QString parent,
                                          const QString index,
                                          const QString& description)
{
    std::string recID = dataRecord::create(recordType.toStdString(),
                                           title.toStdString(),
                                           parent.toStdString(),
                                           index.toStdString(),
                                           description.toStdString());

    Data::setDataValue("ID", "record", recID);
    dataRecord::getRecord();

    std::string mtitle = Data::getDataValue("title", "record");
    std::string mtype = Data::getDataValue("type", "record");
    std::string mparent = Data::getDataValue("parent", "record");
    std::string mdeleted = Data::getDataValue("deleted", "record");
    std::string mstatus = Data::getDataValue("status", "record");
    std::string mtreeViewInex = Data::getDataValue("treeViewIndex", "record");

    Record record;
    record.id = QString::fromStdString(recID);
    record.title = QString::fromStdString(mtitle);
    record.type = QString::fromStdString(mtype);
    record.parent = QString::fromStdString(mparent);
    record.status = QString::fromStdString(mstatus);
    record.deleted = QString::fromStdString(mdeleted);
    record.treeViewIndex = QString::fromStdString(mtreeViewInex);

    recordList.append(record);

    emit recordCreated(QString::fromStdString(recID));

    return QString::fromStdString(recID);
}

void ProjectDataPresenter::deleteRecord(const QString& recID) {
    Data::setDataValue("ID","record",recID.toStdString());
    dataRecord::getRecord();

    dataRecord::remove();

    for (int i=0; i<recordList.count(); i++) {
        if (recordList[i].id == recID) {
            recordList[i].deleted = "true";
            break;
        }
    }

    emit recordDeleted(recID);
}

void ProjectDataPresenter::renameRecord(const QString& recID, const QString& title) {
    Data::setDataValue("ID", "record", recID.toStdString());
    dataRecord::getRecord();
    Data::setDataValue("title", "record", title.toStdString());
}

void ProjectDataPresenter::moveRecord(const QString& recID, const QString& parent, const QString& treeViewIndex)
{
    Data::setDataValue("ID","record",recID.toStdString());
    dataRecord::getRecord();

    Data::setDataValue("parent","record", parent.toStdString());
    Data::setDataValue("treeViewIndex","record", treeViewIndex.toStdString());

    for (int i=0; i<recordList.count(); i++)
    {
        if (recordList[i].id == recID) {
            recordList[i].parent = parent;
            recordList[i].treeViewIndex = treeViewIndex;
            break;
        }
    }
}

RecordCounts ProjectDataPresenter::countsForType(const QString& type)
{
    RecordCounts counts;
    counts.reviewed = 0;
    counts.drafts = 0;
    counts.inReview = 0;

    for (int i=0; i<recordList.count(); i++)
    {
        if (recordList[i].type != type)
            continue;

        if (recordList[i].status == "Draft") {
            counts.drafts++;
        } else if (recordList[i].status == "Review") {
            counts.inReview++;
        } else if (recordList[i].status == "Reviewed") {
            counts.reviewed++;
        }
    }

    return counts;
}
