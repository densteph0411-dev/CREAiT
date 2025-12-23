#include "DocumentRecord.h"
#include <filesystem>

void DocumentRecord::getRecord(std::string ID) {

    // Set the Data::dictionary values
    std::string recordDirectory = Data::getDataValue(this->directory,"project");
    std::string recordFileName = recordDirectory + "/" + ID + ".xml";
    Data::setDataValue("fileName",this->context,recordFileName);
    Data::setDataValue("ID",this->context,ID);

    // define the dictionary data.
    DataDictionary RecordData;

    // Create the record object
    basicRecord DocumentRecord(recordDirectory);
    // Get the dataDictionary and update the main dictionary
    RecordData = DocumentRecord.getData(ID);

    // Set context of dictionary, and merge it into the main
    Data::mergeDataDictionaryIntoMaster(Data::setContexttoValue(RecordData,this->context));
}

std::string DocumentRecord::create(const std::string documentName,
                                   const std::string purpose,
                                   const std::string description,
                                   const std::string agentID) {

    // Create the record file parsing object
    std::string recordDirectory = Data::getDataValue(this->directory,"project");
    basicRecord documentRecord(recordDirectory);

    // Transfer the data dictionary for the type
    std::vector<std::string> dictionaryContext = {this->context};
    std::string newID = documentRecord.create(Data::getDictionaryOfContext(dictionaryContext));
    Data::setDataValue("fileName",this->context,recordDirectory + "/" + newID + ".xml");

    // Set the record ID
    Data::setDataValue("ID",this->context,newID);
    Data::setDataValue("documentName",this->context,documentName);
    Data::setDataValue("purpose",this->context,purpose);
    Data::setDataValue("description",this->context,description);
    Data::setDataValue("agentID",this->context,agentID);
    // Set the creation date
    Data::setDataValue("lastModifiedOn",
                       this->context,
                       Data::time_point_to_string(std::chrono::system_clock::now()));

    // return the new record ID
    return newID;
}

void DocumentRecord::clearDictionaryRecordData() {

    // Get all record keys
    std::vector<std::string> recordKeys = Data::getDictionaryKeysByType(this->context);

    // Iterate through the keys and clear the data. This clears the dictionary directly
    // to avoid clearing the data in the file
    for (const auto &recordKey : recordKeys){
        Data::dictionary->setFieldValue(recordKey,this->context,"");
    }
}

void DocumentRecord::remove(std::string ID) {

    // Lock the record file
    lockRecord(ID);

    Data::setDataValue("deleted",this->context,"true");
}

// Get information for the specified record.
std::string DocumentRecord::getRecordField(const std::string ID,
                                       const std::string field){

    // Create requirement File Parsing Object
    std::string filePath = Data::getDataValue(this->directory, "project") + "/" + ID + ".xml";
    basicRecord parseFile(filePath);

    // Return information from the record
    return parseFile.getRecordField(ID,field);
}

// Get a list of IDs.
std::vector<std::string> DocumentRecord::getListOfRecords() {
    basicRecord parsingRecord(Data::getDataValue(this->directory, "project"));
    const auto paths = parsingRecord.getListofRecords();

    std::vector<std::string> names;
    names.reserve(paths.size());

    for (const auto& s : paths) {
        std::filesystem::path p(s);
        names.emplace_back(p.stem().string()); // filename without extension
    }
    return names;
}

bool DocumentRecord::lockRecord(std::string ID) {
    return true;// COMMENTED Configuration::lockRecord();
}

bool DocumentRecord::releaseRecord(std::string ID) {
    return true;// COMMENTED Configuration::lockRecord();
}
