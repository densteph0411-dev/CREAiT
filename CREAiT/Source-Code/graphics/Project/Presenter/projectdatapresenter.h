#ifndef PROJECTDATAPRESENTER_H
#define PROJECTDATAPRESENTER_H

// #include "Presenter/projectdatapresenter.h"

#include "Presenter/recorddatapresenter.h"
#include <QString>
#include <projecttreemodel.h>


struct Record {
    QString title;
    QString id;
    QString type;
    QString parent;
    QString deleted;
    QString status;
    QString description;
    QString treeViewIndex;
};

struct RecordCounts {
    int reviewed;
    int drafts;
    int inReview;
};

class ProjectDataPresenter : public QObject
{
    Q_OBJECT
public:
    static ProjectDataPresenter& instance();  // singleton accessor

    // Prevent copying
    ProjectDataPresenter(const ProjectDataPresenter&) = delete;
    ProjectDataPresenter& operator=(const ProjectDataPresenter&) = delete;

    QString GetProjectName();
    void SetProjectName(const QString& projectName);

    void loadProject(const QString& projectsPath, const QString& projectName = "");

    QList<Record> fetchAllRecordList();
    QList<Record> fetchChildRecordList(QString parent);
    QList<Record> searchRecordsByKeyword(QString keyword);

    QList<Record> fetchAllActiveRecordList() const;

    QString createRecord(const QString recordType,
                                const QString title,
                                const QString parent,
                                const QString index,
                                const QString& description);
    void renameRecord(const QString& recID, const QString& title);
    void deleteRecord(const QString& recID);
    void moveRecord(const QString& recID, const QString& parent, const QString& treeViewIndex);

    QString getProjectPath();

    static QString getRecordIconPath(Record record);

    QList<RecordType>* getRecordTypeList();
    RecordCounts countsForType(const QString& type);
signals:
    void recordCreated(QString recID);
    void recordDeleted(QString recID);

private:
    ProjectDataPresenter();

    QList<Record> recordList;
    QList<RecordType> recordTypeList;
    QString m_projectPath;
    QString m_projectName;
};

#endif // PROJECTDATAPRESENTER_H
