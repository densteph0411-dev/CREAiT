#include "automationdatacontroller.h"
#include <records.h>

AutomationDataController::AutomationDataController() {}

AutomationData AutomationDataController::loadRecordById(const QString& id, const QString& type) {

    AutomationData automation;

    Data::setDataValue("ID", "automation", id.toStdString());
    // Creating automation record object
    automationRecord automationRecord;
    automationRecord.getRecord(id.toStdString());

    // Adding values to the
    automation.deleted = QString::fromStdString(Data::getDataValue("deleted", "automation"));
    automation.type = QString::fromStdString(Data::getDataValue("type", "automation"));
    if (automation.deleted == "true" || automation.type != type)
        return automation;

    automation.recordID = id;
    automation.title = QString::fromStdString(Data::getDataValue("title", "automation"));
    automation.description = QString::fromStdString(Data::getDataValue("description", "automation"));
    automation.lastModifiedBy = QString::fromStdString(Data::getDataValue("lastModifiedBy", "automation"));
    automation.lastModifiedOn = QString::fromStdString(Data::getDataValue("lastModifiedOn", "automation"));
    automation.version = QString::fromStdString(Data::getDataValue("version", "automation"));
    automation.AIModel = QString::fromStdString(Data::getDataValue("AIModel", "automation"));
    automation.prompt = QString::fromStdString(Data::getDataValue("prompt", "automation"));
    automation.triggerType = QString::fromStdString(Data::getDataValue("triggerType", "automation"));
    automation.durationType = QString::fromStdString(Data::getDataValue("durationType", "automation"));
    automation.duration = QString::fromStdString(Data::getDataValue("duration", "automation"));
    automation.exampleInput_1 = QString::fromStdString(Data::getDataValue("exampleInput_1", "automation"));
    automation.field_1 = QString::fromStdString(Data::getDataValue("field_1", "automation"));
    automation.field_2 = QString::fromStdString(Data::getDataValue("field_2", "automation"));
    automation.field_3 = QString::fromStdString(Data::getDataValue("field_3", "automation"));
    automation.field_4 = QString::fromStdString(Data::getDataValue("field_4", "automation"));
    
    return automation;
}

QMap<QString, AutomationData> AutomationDataController::loadAllRecords(const QString& type) {
    QMap<QString, AutomationData> automations;
    automationRecord automationRecords;

    std::vector<std::string> childRecords = automationRecords.getListOfAutomations();

    for (const auto &recordID : childRecords) {
        QString id = QString::fromStdString(recordID);
        AutomationData automation = AutomationDataController::loadRecordById(id, type);
        automations.insert(id, automation);
    }

    return automations;
}

bool AutomationDataController::deleteRecordById(const QString& id) {

    Data::setDataValue("ID", "automation", id.toStdString());
    automationRecord automationRecord;
    automationRecord.getRecord(id.toStdString());

    // Debug
    std::string recordID = Data::getDataValue("fileName","automation");
    automationRecord.remove(recordID);

    return true;
}

bool AutomationDataController::updateRecord(const AutomationData& automation) {

    automationRecord automationRecord;
    Data::setDataValue("ID", "automation", automation.recordID.toStdString());
    automationRecord.getRecord(automation.recordID.toStdString());
    Data::getDataValue("fileName","automation");

    Data::setDataValue("title", "automation", automation.title.toStdString());
    Data::setDataValue("prompt", "automation", automation.prompt.toStdString());
    Data::setDataValue("AIModel", "automation", automation.AIModel.toStdString());
    Data::setDataValue("version", "automation", automation.version.toStdString());
    Data::setDataValue("triggerType", "automation", automation.triggerType.toStdString());
    Data::setDataValue("durationType", "automation", automation.durationType.toStdString());
    Data::setDataValue("duration", "automation", automation.duration.toStdString());

    return true;
}

QString AutomationDataController::addRecord(const AutomationData& automation) {

    // Clear the record
    QString returnvalue = "";
    automationRecord automationRecord;
    automationRecord.clearDictionaryRecordData();

    // Create the data object
    returnvalue = QString::fromStdString(automationRecord.create("automation",
                                                                 automation.title.toStdString(),
                                                                 automation.description.toStdString()));

    Data::setDataValue("type", "automation", automation.type.toStdString());
    Data::setDataValue("title", "automation", automation.title.toStdString());
    Data::setDataValue("description", "automation", automation.description.toStdString());
    Data::setDataValue("lastModifiedBy", "automation", Data::getDataValue("userID", "project"));
    Data::setDataValue("lastModifiedOn", "automation", Data::time_point_to_string(std::chrono::system_clock::now()));
    Data::setDataValue("version", "automation", automation.version.toStdString());
    Data::setDataValue("AIModel", "automation", automation.AIModel.toStdString());
    Data::setDataValue("prompt", "automation", automation.prompt.toStdString());
    Data::setDataValue("triggerType", "automation", automation.triggerType.toStdString());
    Data::setDataValue("durationType", "automation", automation.durationType.toStdString());
    Data::setDataValue("duration", "automation", automation.duration.toStdString());
    Data::setDataValue("exampleInput_1", "automation", automation.exampleInput_1.toStdString());

    return returnvalue;
}
