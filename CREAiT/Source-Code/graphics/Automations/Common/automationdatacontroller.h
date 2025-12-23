#ifndef AUTOMATIONDATAPRESENTER_H
#define AUTOMATIONDATAPRESENTER_H

#include <QMap>
#include <QString>
#include <QStringList>

struct AutomationData {
    QString recordID;
    QString filename;
    QString type;
    QString description;
    QString title;
    QString deleted;
    QString lastModifiedBy;
    QString status;
    QString lastModifiedOn;
    QString version;
    QString AIModel;
    QString prompt;
    QString triggerType;
    QString durationType;
    QString duration;
    QString exampleInput_1;
    QString field_1;
    QString field_2;
    QString field_3;
    QString field_4;
};

class AutomationDataController
{
public:
    AutomationDataController();

    static AutomationData loadRecordById(const QString& id, const QString& type = "automation");
    static QMap<QString, AutomationData> loadAllRecords(const QString& type = "automation");
    static bool deleteRecordById(const QString& id);
    static bool updateRecord(const AutomationData& automation);
    static QString addRecord(const AutomationData& automation);
};

#endif // AUTOMATIONDATAPRESENTER_H
