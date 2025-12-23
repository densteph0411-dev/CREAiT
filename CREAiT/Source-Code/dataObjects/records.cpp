//---------------------------------------------------------------------------
// $Rev: 507 $: Revision of last commit
// $Author: evanmoyle $ Author of last commit
// $Date: 2025-12-18 09:34:19 -0700 (Thu, 18 Dec 2025) $ Date of last commit
// $HeadURL: svn://75.70.242.166/requirementTool/Software/CREAiT/CREAiT-Qt-GUI/CREAiT/Source-Code/dataObjects/records.cpp $ URL Location
// Copyright 2023-2025
//---------------------------------------------------------------------------

#include "records.h"
#include "generalXMLOperations.h"
#include "projectFileOperations.h"
#include "recordFileOperations.h"
#include "generalFileOperations.h"
#include "reviewFileOperations.h"
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <QDir>


// Define and initialize the static member
DataDictionary* Data::dictionary = new DataDictionary();  // Make sure to initialize it properly

std::vector<std::string> dataRecord::linkName;
std::vector<std::string> dataRecord::linkDirection;
std::vector<std::string> dataRecord::linkType;
std::vector<std::string> dataRecord::linkTitle;
std::string dataRecord::targetLinkID;

std::vector<std::string> dataRecord::versionNumber;
std::vector<std::string> dataRecord::versionDescription;
std::vector<std::string> dataRecord::versionDate;

void Data::addData(const std::string fieldName,
                   const std::string context,
                   const std::string fieldValue,
                   const std::string persistence,
                   const std::string description) {

    // Check if the field already exists. If so, set it, if not, create it
    if(Data::dictionary->keyExists(fieldName,context)){
        Data::setDataValue(fieldName,context,fieldValue);
    }
    else
    {
        Data::dictionary->createField(fieldName, context, fieldValue, persistence, description);

        //Test if the data is persistent. If so, set it in persistent memory
        if(persistence == "true"){

            // Get the project Name
            std::string fileName = Data::getDataValue("fileName",context);
            std::string projectName = "";

            // Verify the fileName is present
            if(fileName != ""){

                // Test if this is a project file, or record. If it's a project file,
                // set the projectName, otherwise leave it blank
                if(isAProjectFile(fileName)){
                    projectName = Data::getDataValue("projectName","project");
                }

                // Create the new filed in the file
                fileOperations::generalXMLOperations generalFile;
                generalFile.createField(fieldName,
                                        fieldValue,
                                        fileName,
                                        projectName);
            }
        }
    }
}

// Tests of the data type is a record or not
bool Data::isAProjectFile(std::string fileName){

    bool testVal = false;

    // Get the directory path by removing the filename from the path and
    // comparing to see if it's located in the program directory.
    std::string currentFilePath = std::filesystem::path(fileName).parent_path().string();
    std::string programDirectory = Data::getDataValue("programDirectory","project");
    if(currentFilePath == programDirectory){
        testVal = true;
    }

    return testVal;
}


void Data::setDataValue(const std::string fieldName,
                        const std::string context,
                        const std::string fieldValue) {
    Data::dictionary->setFieldValue(fieldName, context, fieldValue);

    if(Data::dictionary->getFieldPersistence(fieldName,context) == "true"){

        // Test if a filename is present. If not, dont save it
        std::string fileName = Data::getDataValue("fileName", context);
        std::string description = Data::getDataDescription(fieldName,context);

        // Get the project name so the parser knows which project to add this to.
        std::string projectName = Data::getDataValue("projectName", "project");

        // If a file name is present, update the fieldtype
        if(fileName != ""){
            fileOperations::generalXMLOperations generalFile;
            generalFile.setFieldValue(fieldName,
                                      fieldValue,
                                      description,
                                      fileName,
                                      projectName,
                                      context);
        }
    }
}

void Data::removeData(const std::string fieldKey, const std::string context) {
    Data::dictionary->removeField(fieldKey, context);

    fileOperations::generalXMLOperations generalFile;
    generalFile.removeFieldInXMLFile(fieldKey,
                                     getDataValue("fileName", context),
                                     context,
                                     Data::getDataValue("projectName","project"));
}

std::string Data::getDataValue(const std::string fieldName, const std::string context) {

    return Data::dictionary->getFieldValue(fieldName, context);
}

std::string Data::getDataPersistence(const std::string key, const std::string context) {
    return Data::dictionary->getFieldPersistence(key, context);
}

std::string Data::getDataDescription(const std::string key, const std::string context) {
    return Data::dictionary->getFieldDescription(key, context);
}

void Data::clearDataDictionaryByContext(const std::string context) {
    Data::dictionary->eraseDataTypeByContext(context);
}

bool Data::keyExists(const std::string key, const std::string context) {
    return Data::dictionary->keyExists(key, context);
}

void Data::mergeDataDictionaryIntoMaster(DataDictionary data)
{
    if (!Data::dictionary) return; // safety

    // Walk every context in 'data'
    const std::vector<std::string> contexts = data.getAllContexts();
    for (const auto& ctx : contexts)
    {
        // Walk every key in the context
        const std::vector<std::string> keys = data.getAllKeysByContext(ctx);
        for (const auto& key : keys)
        {
            const std::string value       = data.getFieldValue(key, ctx);
            const std::string persistence = data.getFieldPersistence(key, ctx);
            const std::string description = data.getFieldDescription(key, ctx);

            if (Data::dictionary->keyExists(key, ctx))
            {
                // Update the value
                Data::dictionary->setFieldValue(key, ctx, value);

                // If metadata differs, refresh it by recreating the field
                const bool metaDiffers =
                    (Data::dictionary->getFieldPersistence(key, ctx) != persistence) ||
                    (Data::dictionary->getFieldDescription(key, ctx)  != description);

                if (metaDiffers)
                {
                    Data::dictionary->removeField(key, ctx);
                    Data::dictionary->createField(key, ctx, value, persistence, description);
                }
            }
            else
            {
                // Create the new field with full metadata
                Data::dictionary->createField(key, ctx, value, persistence, description);
            }
        }
    }
}

DataDictionary Data::getDictionaryOfContext(const std::vector<std::string> contexts) {
    DataDictionary result;

    // Iterate over each context in the input vector
    for (const auto& context : contexts) {
        // Get all keys that belong to the current context
        const std::vector<std::string> keys = Data::dictionary->getAllKeysByContext(context);

        // For each key, copy over its data into the result dictionary
        for (const auto& key : keys) {
            // Only include keys that actually exist (defensive check)
            if (Data::dictionary->keyExists(key, context)) {
                const std::string value        = Data::dictionary->getFieldValue(key, context);
                const std::string persistence  = Data::dictionary->getFieldPersistence(key, context);
                const std::string description  = Data::dictionary->getFieldDescription(key, context);

                result.createField(key, context, value, persistence, description);
            }
        }
    }

    return result;
}

std::vector<std::string> Data::getDictionaryKeysByType(const std::string context) {
    return Data::dictionary->getAllKeysByContext(context);
}

std::string Data::time_point_to_string(const std::chrono::system_clock::time_point& time_point,
                                       const std::string& format) {
    std::time_t tt = std::chrono::system_clock::to_time_t(time_point);
    std::tm tm;

#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm, &tt);  // Windows safe version
#else
    localtime_r(&tt, &tm);  // POSIX safe version
#endif

    std::stringstream ss;
    ss << std::put_time(&tm, format.c_str());
    return ss.str();
}

DataDictionary Data::setContexttoValue(DataDictionary data, std::string context)
{
    DataDictionary result;

    // Walk every existing context in the provided dictionary
    const std::vector<std::string> contexts = data.getAllContexts();
    for (const auto& ctx : contexts)
    {
        // For each key in that context, recreate it under the new target context
        const std::vector<std::string> keys = data.getAllKeysByContext(ctx);
        for (const auto& key : keys)
        {
            const std::string value       = data.getFieldValue(key, ctx);
            const std::string persistence = data.getFieldPersistence(key, ctx);
            const std::string description = data.getFieldDescription(key, ctx);

            result.createField(key, context, value, persistence, description);
        }
    }

    return result;
}


void dataRecord::getRecord() {

    std::string recordDirectory = Data::getDataValue("recordDirectory","project");
    std::string recordID = Data::getDataValue("ID","record");

    // Clear the old record data
    dataRecord::clearDictionaryRecordData();

    // Set the Data::dictionary values
    Data::setDataValue("ID","record",recordID);
    std::string recordFileName = recordDirectory + "/" + recordID + ".xml";
    Data::setDataValue("fileName","record",recordFileName);

    fileOperations::parseRecordFile parseFile(recordDirectory);
    // Copy data dictionary
    parseFile.fieldInformationDictionary = *Data::dictionary;
    parseFile.readRecordFile(Data::getDataValue("ID","record"));

    // Clear all of the link information
    dataRecord::linkName.clear();
    dataRecord::linkType.clear();
    dataRecord::linkDirection.clear();
    dataRecord::linkTitle.clear();
    // Clear Version History
    dataRecord::versionNumber.clear();
    dataRecord::versionDescription.clear();
    dataRecord::versionDate.clear();

    // Copy the dictionary data back
    *Data::dictionary = parseFile.fieldInformationDictionary;
    dataRecord::linkName = parseFile.linkName;
    dataRecord::linkType = parseFile.linkType;
    dataRecord::linkDirection = parseFile.linkDirection;
    dataRecord::linkTitle = parseFile.linkTitle;

    // Copy the version information
    dataRecord::versionNumber = parseFile.versionNumber;
    dataRecord::versionDate = parseFile.versionDate;
    dataRecord::versionDescription = parseFile.versionDescription;

    // Reload
    Data::setDataValue("fileName","record",recordFileName);
}

std::vector<std::string> Data::getAllContexts(void) {
    return Data::dictionary->getAllContexts();
}

std::string dataRecord::create(const std::string recordType,
                               const std::string title,
                               const std::string parent,
                               const std::string index,
                               const std::string& description) {

    std::string previousID = Data::getDataValue("ID", "record");
    dataRecord::clearDictionaryRecordData();

    Data::setDataValue("type", "record", recordType);
    Data::setDataValue("title", "record", title);
    Data::setDataValue("description", "record", description);
    Data::setDataValue("status", "record", "Draft");
    Data::setDataValue("lastModifiedBy", "record", Data::getDataValue("userID", "project"));
    Data::setDataValue("lastModifiedOn", "record", Data::time_point_to_string(std::chrono::system_clock::now()));
    Data::setDataValue("version", "record", "1");
    Data::setDataValue("parent", "record", parent);
    Data::setDataValue("treeViewIndex", "record", index);

    // Create the project object, and get the next available ID
    fileOperations::parseProjectFile parseProjectFile(Data::getDataValue("programDirectory","project"));

    // Set the project name
    std::string nextID = parseProjectFile.getNextID(recordType);

    // Create the record file parsing object
    std::string recordDirectory = Data::getDataValue("recordDirectory","project");
    fileOperations::parseRecordFile parseRecordFile(recordDirectory);
    // Transfer the dictionary data
    parseRecordFile.fieldInformationDictionary = *Data::dictionary;
    std::string newRecordID = parseRecordFile.createRecordFile(recordDirectory,recordType,nextID);

    // Make sure there is a new record created. If an ID was returned, then
    // the record was correctly created
    if (newRecordID.empty()) {
        return "";
    }

    // Copy the dictionary back
    *Data::dictionary = parseRecordFile.fieldInformationDictionary;
    // Test the record ID
    std::string testID = Data::getDataValue("ID","record");

    // return the new record ID
    return newRecordID;
}

void dataRecord::edit() {

    // Lock the record in CM
    lockRecord();

    // Create the record file parsing object
    fileOperations::parseRecordFile parseRecordFile(Data::getDataValue("recordDirectory","project"));

    // UPdate the timestamp and user information
    Data::setDataValue("lastModifiedBy", "record", Data::getDataValue("userID", "project"));
    Data::setDataValue("lastModifiedOn", "record", Data::time_point_to_string(std::chrono::system_clock::now()));

    parseRecordFile.editRecordFile();
    // COMMENTED Configuration::commitRecord();
    // COMMENTED Configuration::commitProjectFile();
}

void dataRecord::clearDictionaryRecordData() {

    // Get all record keys
    std::vector<std::string> recordKeys = Data::getDictionaryKeysByType("record");

    // Iterate through the keys and clear the data. This clears the dictionary directly
    // to avoid clearing the data in the file
    for (const auto &recordKey : recordKeys){
        Data::dictionary->setFieldValue(recordKey, "record","");
    }
}

void dataRecord::remove(void) {

    // Lock the record file
    lockRecord();

    Data::setDataValue("deleted","record","true");
}

// Add links to the record.
void dataRecord::addLink(std::string reqID,
                         bool linkIsUp,
                         std::string linkType){

    // Test if the link is already present
    if (reqID == Data::getDataValue("ID", "record")) {
        return;
    }

    // Store the current record to reload after adding
    // link to the reqID record file
    std::string currentRecordID = Data::getDataValue("ID", "record");

    // If no current record exists, abort
    if(currentRecordID == ""){
        return;
    }

    // Create requirement File Parsing Object
    fileOperations::parseRecordFile parseRecordFile(Data::getDataValue("recordDirectory","project"));
    parseRecordFile.fieldInformationDictionary = *Data::dictionary;

    // Add the link, passed in to the current record
    parseRecordFile.linkManipulate(reqID, true, linkIsUp, linkType);

    if(linkIsUp){
        linkDirection.push_back("Up");
    }
    else
    {
        linkDirection.push_back("Down");
    }

    // Add the other side of the link, to the linked record
    Data::setDataValue("ID", "record", reqID);
    dataRecord::getRecord();

    // Add the other side of the link to the linked record file
    parseRecordFile.fieldInformationDictionary = *Data::dictionary;
    parseRecordFile.linkManipulate(currentRecordID, true, !linkIsUp, linkType);

    // reload the original record
    // This also reloads all of the links
    Data::setDataValue("ID", "record", currentRecordID);
    dataRecord::getRecord();
}

// Remove links from the record
void dataRecord::removeLink(const std::string& reqID) {

    // Store the current record to reload after adding
    // link to the reqID record file
    std::string currentRecordID = Data::getDataValue("ID", "record");

    // Create requirement File Parsing Object
    fileOperations::parseRecordFile parseRecordFile(Data::getDataValue("recordDirectory","project"));
    parseRecordFile.fieldInformationDictionary = *Data::dictionary;
    // Add the link, passing in
    parseRecordFile.linkManipulate(reqID, false, false, "");

    // Add the other side of the link, to the linked record
    Data::setDataValue("ID", "record", reqID);
    dataRecord::getRecord();

    // Add the other side of the link to the linked record file
    parseRecordFile.fieldInformationDictionary = *Data::dictionary;
    parseRecordFile.linkManipulate(currentRecordID, false, false, "");

    // Reload the original record which also reloads the links
    Data::setDataValue("ID", "record", currentRecordID);
    dataRecord::getRecord();
}

// Get information for the specified record.
std::string dataRecord::getRecordField(const std::string& recordID,
                                       const std::string& field){

    // Create requirement File Parsing Object
    fileOperations::parseRecordFile parseRecordFile(Data::getDataValue("recordDirectory","project"));

    // Return information from the record
    return parseRecordFile.getRecordInformation(recordID, field);
}

bool dataRecord::lockRecord() {
    return true;// COMMENTED Configuration::lockRecord();
}

void dataRecord::releaseRecord() {
    // COMMENTED Configuration::releaseRecordLock();
}

// Updates the current project based on the projectName
bool projectInfoRecord::load(std::string projectFileName, std::string projectName) {

    // Define the return value variable
    bool returnValue = false;

    // Clear the project data to prepare for the new project
    projectInfoRecord::clearData();

    // Set up all of the directory paths if they don't already exist
    projectInfoRecord::setUpDirectoryAndFileValues(projectFileName,projectName);

    // create project file parsing object and load the project file
    fileOperations::parseProjectFile parseProjectFile(Data::getDataValue("programDirectory","project"));
    parseProjectFile.activeProjectName = projectName;
    // Copy over the dictionary
    parseProjectFile.fieldInformationDictionary = *Data::dictionary;
    parseProjectFile.load();

    // Get the dictionary data information loaded from the project files
    *Data::dictionary = parseProjectFile.fieldInformationDictionary;

    // Test that at least one context is present
    static std::vector<std::string> contexts = Data::getAllContexts();

    // Check that data was loaded (Not empty)
    if(!contexts.empty()){
        // Data was loaded
        returnValue = true;
    }

    return returnValue;
}

// The following data needs to be set before calling this method to
// create a new project. Otherwise it will not be created
bool projectInfoRecord::createNew(std::string baseDirectory,
                                  std::string projectName,
                                  std::string type){

    // Define the return value
    bool returnVal = false;

    // Set the type and project name
    Data::setDataValue("type","project",type);
    Data::setDataValue("projectName","project",projectName);

    // Get the project information
    std::string homeDirectoryLocation = "";
    std::string programDirectory = "";
    std::string projectDirectory = "";
    std::string projectFileName = "";
    std::string tempFilePath = "";

    // Check if a project name exists
    if(projectName.empty()){
        std::cerr << "projectInfoRecord-createNew-Error: Could not load project file. Missing projectName" << std::endl;
        return false;
    }

    // Statement to handle type of svn, local, or no type specified.
    // it creates a local directory structure
    if((type == "svn") or (type == "local") or !type.empty()){

        // Check if a base directory location exists
        if(baseDirectory.empty()){

            // Get the home directory
            homeDirectoryLocation = projectInfoRecord::getHomeDirectory();

            // If the home directory is empty, no location available
            if(homeDirectoryLocation.empty()){
                std::cerr << "projectInfoRecord-createNew-Error: Cannot determine home directory location. Aborting project creation:" +
                                 homeDirectoryLocation << std::endl;
                return false;

            }else{
                std::cerr << "projectInfoRecord-createNew-Error: Using the default location of home directory" +
                                 homeDirectoryLocation << std::endl;
                baseDirectory = homeDirectoryLocation;
            }

            std::cerr << "projectInfoRecord-createNew-Error: Using default program location directory location in Home directory" << std::endl;
        }

        // Set the basedirectory and project name
        Data::setDataValue("baseDirectory","project",baseDirectory);

        // Set Program Directory
        programDirectory = baseDirectory + "/" + Data::getDataValue("toolName","project");

        // projectFileName is Progeam directory path and project file name
        projectFileName = programDirectory + "/" + Data::getDataValue("fileNameOnly","project");

        // Set up the project directories
        projectInfoRecord::setUpDirectoryAndFileValues(projectFileName,projectName);

        // create project file parsing object and pass in the programDirectory
        fileOperations::parseProjectFile parseProjectFile(programDirectory);
        parseProjectFile.fieldInformationDictionary = *Data::dictionary;

        // Add the project to the new files.
        if (parseProjectFile.addProjectToProjectFile(Data::getDictionaryOfContext(projectFileList)))
        {

            // Set the active project to the new one
            parseProjectFile.setActiveProject(projectName,Data::getDataValue("fileName","credentials"));

            // Add the project node to the dictionary as the first tree and set the record ID to "project"
            Data::setDataValue("ID",
                               "record",
                               dataRecord::create("project",
                                                  Data::getDataValue("projectName", "project"),
                                                  "",
                                                  "",
                                                  Data::getDataValue("description", "project")));

            // Get all of the recordTypes and innerText
            std::string title;
            std::vector<std::string> allRecordTypes = Data::getDictionaryKeysByType("recordType");

            // Index containing where in the parent the record should be
            // first index is blank as it's the first child
            std::string tempTreeViewIndex = "";
            std::string previousChild = "";

            // Add record Types as folders to the project
            // Test of recordTypes is empty
            if ((!allRecordTypes.empty())) {
                // Add directories for each record type
                for (const auto& recordType : allRecordTypes) {

                    // Get the innert text as the title
                    title = Data::getDataValue(recordType,"recordType");

                    // Don't create a folder for Directories
                    if(recordType != "DIR"){
                        previousChild = dataRecord::create("Directory",title,"PROJ",previousChild,Data::getDataDescription(recordType,"recordType"));
                    }
                }

                returnVal = true;
            }
        }

        // Open and update the new project file and the treeView Info
        projectInfoRecord::load(projectFileName,projectName);
    }

    return returnVal;
}

// Clears the project information stored in the static buffer for the project context
void projectInfoRecord::clearData(void) {

    // Store Temporary Path Values. If these change they need reset by the method calling them
    std::string baseDirectory = Data::getDataValue("baseDirectory","project");
    std::string programDirectory = Data::getDataValue("programDirectory","project");
    std::string projectFileName = Data::getDataValue("fileName","project");

    // Re initialize all of the project data
    projectInfoRecord::initialize();

    // Reset the needed constants
    Data::setDataValue("baseDirectory","project",baseDirectory);
    Data::setDataValue("programDirectory","project",programDirectory);
    Data::setDataValue("fileName","project",projectFileName);
}

// Clears the project information stored in the static buffer
void projectInfoRecord::changeProject(void) {

    // create requirement file parsing object
    fileOperations::parseProjectFile parseProjectFile(Data::getDataValue("fileName","project"));

    parseProjectFile.setActiveProject(Data::getDataValue("projectName", "project"),
                                      Data::getDataValue("fileName", "credentials"));

    parseProjectFile.fieldInformationDictionary = *Data::dictionary;
    parseProjectFile.updateProject(Data::getDataValue("projectName", "project"));
}

// Initialize the dataRecord dictionary
void projectInfoRecord::initialize(void) {

    Data::dictionary->eraseDataTypeByContext("record");
    Data::dictionary->eraseDataTypeByContext("project");
    Data::dictionary->eraseDataTypeByContext("credentials");
    Data::dictionary->eraseDataTypeByContext("linkType");
    Data::dictionary->eraseDataTypeByContext("recordType");
    Data::dictionary->eraseDataTypeByContext("automation");
    Data::dictionary->eraseDataTypeByContext("userInfo");
    Data::dictionary->eraseDataTypeByContext("connection");
    Data::dictionary->eraseDataTypeByContext("review");

    // project - Add the configuration records to the dictionary
    Data::addData("fileName", "project", "", "false", "The path of the project file");
    Data::addData("fileNameOnly", "project", "project.xml", "true", "The project filename");

    // project - Basic Project Information
    Data::addData("projectName", "project", "", "true", "The name of the project");
    Data::addData("type", "project", "", "true", "The type of the project");
    Data::addData("description", "project", "", "true", "The description of the project");

    // project - Directory information
    Data::addData("baseDirectory", "project", "", "false", "The base directory of the project");
    Data::addData("programDirectory", "project", "", "false", "The directory of the program");
    Data::addData("projectDirectory", "project", "", "false", "The directory of the loaded project within the program");

    // project - Database Information
    Data::addData("CMUrl", "project", "NA", "false", "The database name");

    // project - AI projectInformation
    Data::addData("AIConnectionID", "project", "", "true", "ID of the current AI Connection");
    Data::addData("IntegrationID", "project", "", "true", "ID of the current selected integration");

    // project - Database information
    Data::addData("dataBaseName", "project", "", "true", "The name of the database");
    Data::addData("dataBaseUrl", "project", "", "true", "The URL of the database");
    Data::addData("toolName", "project", "CREAiT", "true", "The URL of the configuration management system");
    Data::addData("IDPrefix", "project", "", "true", "The prefix for the record ID");

    // credentials
    Data::addData("fileName", "credentials", "", "true", "The path of the credentials file");
    Data::addData("fileNameOnly", "credentials", "credentials.xml", "true", "The credentials filename");
    Data::addData("userID", "credentials", "", "true", "Login UserName");
    Data::addData("passWord", "credentials", "", "true", "Login Password");
    Data::addData("AIkey", "credentials", "", "true", "AI key");
    Data::addData("email", "credentials", "", "true", "User email");

    // userInfo
    Data::addData("ID", "userInfo", "", "false", "ID");
    Data::addData("userName", "userInfo", "", "true", "Username");
    Data::addData("email", "userInfo", "", "true", "User email address");
    Data::addData("permissions", "userInfo", "", "true", "User Permission Status");
    Data::addData("fileName", "userInfo", "", "true", "User Permission Status");
    Data::addData("lastModifiedOn", "userInfo", "", "true", "When the record was created");

    // linkType
    Data::addData("fileName", "linkType", "", "true", "The path of the file");
    Data::addData("fileNameOnly", "linkType", "linkTypes.xml", "true", "The filename holding this information");
    Data::addData("Traces", "linkType", "", "true", "The prefix for the record ID");
    Data::addData("Allocates", "linkType", "", "true", "The prefix for the record ID");
    Data::addData("Satisfies", "linkType", "", "true", "The prefix for the record ID");
    Data::addData("Associates", "linkType", "", "true", "The prefix for the record ID");

    // document
    Data::addData("ID", "document", "", "false", "The path of the file");
    Data::addData("fileName", "document", "", "true", "Document Record Filename");
    Data::addData("documentName", "document", "", "true", "The document Name");
    Data::addData("purpose", "document", "", "true", "The prefix for the record ID");
    Data::addData("description", "document", "", "true", "The prefix for the record ID");
    Data::addData("lastModifiedOn", "document", "", "true", "The date and time the document was uploaded");
    Data::addData("AgentID", "document", "", "true", "The ID of the agent");
    Data::addData("AIfileID", "document", "", "true", "The AI fileID");
    Data::addData("VectorStoreId", "document", "", "true", "The AI vector store ID");

    // recordType
    Data::addData("DIR", "recordType", "Directory", "true", "Directories that contain records");
    Data::addData("fileName", "recordType", "", "false", "The filename holding this information");
    Data::addData("SyREQ", "recordType", "System Requirement", "true", "The prefix for the record ID");
    Data::addData("SyTest", "recordType", "System Test", "true", "The prefix for the record ID");
    Data::addData("SyTC", "recordType", "System Test Case", "true", "The prefix for the record ID");
    Data::addData("SyTP", "recordType", "System Test Procedure", "true", "The prefix for the record ID");
    Data::addData("SwREQ", "recordType", "Software Requirement", "true", "The prefix for the record ID");
    Data::addData("SwTest", "recordType", "Software Test", "true", "The prefix for the record ID");
    Data::addData("SwTC", "recordType", "Software Test Case", "true", "The prefix for the record ID");
    Data::addData("SwTP", "recordType", "Software Test Procedure", "true", "The prefix for the record ID");
    Data::addData("HwREQ", "recordType", "Hardware Requirement", "true", "The prefix for the record ID");
    Data::addData("HwTest", "recordType", "Hardware Test", "true", "The prefix for the record ID");
    Data::addData("HwTC", "recordType", "Hardware Test Case", "true", "The prefix for the record ID");
    Data::addData("HwTP", "recordType", "Hardware Test Procedure", "true", "The prefix for the record ID");
    Data::addData("DOC", "recordType", "Document", "true", "The prefix for the record ID");
    Data::addData("Code", "recordType", "Code", "true", "The prefix for the record ID");
    Data::addData("DRW", "recordType", "Drawing", "true", "The prefix for the record ID");
    Data::addData("MOD", "recordType", "Model", "true", "The prefix for the record ID");

    // record - ID is not persistent because it changes dynamically
    Data::addData("ID", "record", "", "false", "The record ID");
    Data::addData("fileName", "record", "", "true", "The record fileName");
    Data::addData("type", "record", "", "true", "The record type");
    Data::addData("title", "record", "", "true", "The record title");
    Data::addData("deleted", "record", "false", "true", "The record deletion status");
    Data::addData("description", "record", "", "true", "The record description");
    Data::addData("status", "record", "", "true", "The record status");
    Data::addData("lastModifiedBy", "record", "", "true", "The record lastModifiedBy");
    Data::addData("lastModifiedOn", "record", "", "true", "The record lastModifiedOn");
    Data::addData("version", "record", "", "true", "The record version");
    Data::addData("parent", "record", "", "true", "The parent to the record");
    Data::addData("treeViewIndex", "record", "", "true", "The index of the parent of the record");

    // automation
    Data::addData("ID", "automation", "", "false", "The automation ID");
    Data::addData("fileName", "automation", "", "true", "The automation fileName");
    Data::addData("type", "automation", "", "true", "The automation type");
    Data::addData("title", "automation", "", "true", "The automation title");
    Data::addData("deleted", "automation", "false", "true", "The automation deletion status");
    Data::addData("description", "automation", "", "true", "The automation description");
    Data::addData("status", "automation", "", "true", "The automation status");
    Data::addData("lastModifiedBy", "automation", "", "true", "The automation lastModifiedBy");
    Data::addData("lastModifiedOn", "automation", "", "true", "The automation lastModifiedOn");
    Data::addData("version", "automation", "", "true", "The automation version");
    Data::addData("AIModel", "automation", "", "true", "The automation AIModel");
    Data::addData("AIVectorStoreID", "automation", "", "true", "The Vector ID");
    Data::addData("AIAgentID", "automation", "", "true", "The AI Agent ID for the connection");
    Data::addData("prompt", "automation", "", "true", "The automation prompt");
    Data::addData("triggerType", "automation", "", "true", "The automation triggerType");
    Data::addData("durationType", "automation", "", "true", "The automation durationType");
    Data::addData("duration", "automation", "", "true", "The automation duration");
    // These are created as needed for each dataset. For example exampleInput_2 etc...
    Data::addData("exampleInput_1", "automation", "", "true", "The automation exampleInput_1");

    // connection
    Data::addData("ID","connection","","true","Integration ID");
    Data::addData("type","connection","","true","Connection Type");
    Data::addData("ToolName","connection","","true","AI Model Type");
    Data::addData("url","connection","","true","Tool Type");
    Data::addData("ModelOrVersion","connection","","true","AI Model or Version");
    Data::addData("Key","connection","","true","AI Key");
    Data::addData("fileName","connection","","true","The filename");
    Data::addData("type", "connection", "", "true", "The connection type");
    Data::addData("description", "connection", "", "true", "The description");
    Data::addData("lastModifiedBy", "connection", "", "true", "User that modified this connection");
    Data::addData("lastModifiedOn", "connection", "", "true", "Date the connection was modified on");

    // review
    Data::addData("ID", "review", "", "false", "The review ID");
    Data::addData("fileName", "review", "", "true", "The review fileName");
    Data::addData("type", "review", "", "true", "The review type");
    Data::addData("title", "review", "", "true", "The review title");
    Data::addData("deleted", "review", "false", "true", "The review deletion status");
    Data::addData("description", "review", "", "true", "The review description");
    Data::addData("status", "review", "", "true", "The review status");
    Data::addData("lastModifiedBy", "review", "", "true", "The review lastModifiedBy");
    Data::addData("lastModifiedOn", "review", "", "true", "The review lastModifiedOn");
    Data::addData("version", "review", "", "true", "The review version");
    Data::addData("AIModel", "review", "", "true", "The review AIModel");
    Data::addData("prompt", "review", "", "true", "The review prompt");
    Data::addData("triggerType", "review", "", "true", "The review triggerType");
    Data::addData("durationType", "review", "", "true", "The review durationType");
    Data::addData("duration", "review", "", "true", "The review duration");
    // These are created as needed for each dataset. For example exampleInput_2 etc...
    Data::addData("exampleInput_1", "review", "", "true", "The review exampleInput_1");}

std::vector<std::string> projectInfoRecord::getAllRecords(void){

    // Create the file object
    std::string programDIR = Data::getDataValue("programDirectory","project");
    fileOperations::parseProjectFile parseProjectFile(programDIR);
    parseProjectFile.activeProjectName = Data::getDataValue("projectName","project");
    parseProjectFile.fieldInformationDictionary = *Data::dictionary;

    return parseProjectFile.getRecordList();
}

std::vector<std::string> projectInfoRecord::getAllAutomations(void){

    // Create the file object
    std::string programDIR = Data::getDataValue("programDirectory","project");
    fileOperations::parseProjectFile parseProjectFile(programDIR);
    parseProjectFile.activeProjectName = Data::getDataValue("projectName","project");
    parseProjectFile.fieldInformationDictionary = *Data::dictionary;

    return parseProjectFile.getAutomationsList();
}

std::vector<std::string> projectInfoRecord::getAllReviews(void){

    // Create the file object
    std::string programDIR = Data::getDataValue("programDirectory","project");
    fileOperations::parseProjectFile parseProjectFile(programDIR);
    parseProjectFile.activeProjectName = Data::getDataValue("projectName","project");
    parseProjectFile.fieldInformationDictionary = *Data::dictionary;

    return parseProjectFile.getAutomationsList();
}

// Returns an array of all Integrations in the project
std::vector<std::string> projectInfoRecord::getAllIntegrations(void){

    std::vector<std::string> connections;
    connections = {"DOORs","JAMA","Polarion"};

    return connections;
}

std::string projectInfoRecord::getHomeDirectory(void) {
    return QDir().homePath().toStdString();
}

std::vector<std::string> projectInfoRecord::getRecentProjects(){

    // Create the file object
    fileOperations::parseProjectFile parseProjectFile(Data::getDataValue("programDirectory","project"));

    // Get the file list
    parseProjectFile.getProjectList();

    return parseProjectFile.projectList;
}

// Set up dictionary values for directories
void projectInfoRecord::setUpDirectoryAndFileValues(std::string projectFileName, std::string projectName){

    std::string tempBaseDirectory = "";
    std::string tempProgramDirectory = "";
    std::string tempProjectDirectory = "";
    std::string tempRecordDirectory = "";
    std::string tempfileName = "";

    // Check for projectFileName validity
    if((projectFileName == "") or (!projectFileName.ends_with(".xml"))){
        std::cout << "setUpDirectoryAndFileValues: No project file exists" << std::endl;
        return;
    }

    // Set the project name
    Data::setDataValue("projectName","project",projectName);
    Data::setDataValue("fileName","project",projectFileName);

    // Set the program directory with path returned after removing the project file name
    // This directory holds all of the project files which
    // First test if a value is present
    tempProgramDirectory = projectInfoRecord::removeString(projectFileName, "/" + Data::getDataValue("fileNameOnly","project"));
    Data::setDataValue("programDirectory","project", tempProgramDirectory);

    // Set the base directory by removing the toolnam
    tempBaseDirectory = projectInfoRecord::removeString(tempProgramDirectory, "/" + Data::getDataValue("toolName","project"));
    Data::setDataValue("baseDirectory","project", tempBaseDirectory);

    // Set the project directory
    tempProjectDirectory = tempProgramDirectory + "/" + Data::getDataValue("projectName","project");
    Data::setDataValue("projectDirectory","project", tempProjectDirectory);

    // create project file parsing object and pass in the programDirectory
    fileOperations::parseProjectFile parseProjectFile(tempProgramDirectory);

    // Pass in the data to be added to the project files
    parseProjectFile.fieldInformationDictionary = *Data::dictionary;
    // if the directory was successfully created, then add the project to the project file
    if (!parseProjectFile.setUpProjectDirAndFile(tempProjectDirectory,
                                                 recordList,
                                                 projectFileList)){

        // Was not successfully created
        return;
    }

    // Add the recordTypes
    for (auto& recordType : recordList) {

        // Define the record Directory
        tempRecordDirectory = tempProjectDirectory + "/" + recordType;

        // Check of the directory key exists, if so set it, if not create it
        if(Data::keyExists(recordType + "Directory","project")){
            Data::setDataValue(recordType + "Directory","project",tempRecordDirectory);
        }
        else
        {
            Data::addData(recordType + "Directory", "project", tempRecordDirectory, "false", "The filepath of the " + recordType + " directory");
        }
    }
    std::string testVal;

    // update the project filename values
    for (auto& context : projectFileList) {

        // Check that the value is not set
        // Define the record Directory
        tempfileName = tempProgramDirectory + "/" + context + ".xml";

        Data::setDataValue("fileName",context,tempfileName);
        testVal = Data::getDataValue("fileName",context);
    }

    testVal = Data::getDataValue("baseDirectory","project");
    testVal = Data::getDataValue("programDirectory","project");
    testVal = Data::getDataValue("projectDirectory","project");
    testVal = Data::getDataValue("recordDirectory","project");
    testVal = Data::getDataValue("connectionDirectory","project");
    testVal = Data::getDataValue("documentDirectory","project");
    testVal = Data::getDataValue("imageDirectory","project");
    testVal = Data::getDataValue("reviewDirectory","project");
    testVal = Data::getDataValue("userDirectory","project");

    testVal = Data::getDataValue("fileName","project");
    testVal = Data::getDataValue("fileName","recordType");
    testVal = Data::getDataValue("fileName","linkType");
    testVal = Data::getDataValue("fileName","credentials");
}

std::string projectInfoRecord::removeString(std::string inputString, std::string stringToRemove){

    // Define the return string
    std::string returnString = "";

    // Find the position of the CREAiT directory
    size_t pos = inputString.rfind(stringToRemove);

    // Parse out the baseDirectory
    if (pos != std::string::npos) {
        // Extract the path up to (but not including) the CREAiT directory
        returnString = inputString.substr(0, pos);

    } else {
        std::cout << "Directory 'CREAiT' not found in path." << std::endl;
    }

    return returnString;
}

std::string projectInfoRecord::generateUUIDv4() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist;

    uint64_t part1 = dist(gen);
    uint64_t part2 = dist(gen);

    std::ostringstream oss;
    oss << std::hex << std::setfill('0')
        << std::setw(8) << ((part1 >> 32) & 0xFFFFFFFF) << "-"
        << std::setw(4) << ((part1 >> 16) & 0xFFFF) << "-"
        << std::setw(4) << ((part1 & 0x0FFF) | 0x4000) << "-"         // version 4
        << std::setw(4) << (((part2 >> 48) & 0x3FFF) | 0x8000) << "-"  // variant
        << std::setw(12) << (part2 & 0xFFFFFFFFFFFFULL);

    return oss.str();
}

// Get the image file for the record Types
std::string projectInfoRecord::getRecordTypeImageFilePath(std::string recordType){

    std::string returnVal = "";

    // If directory path is not empty, set up the image path
    if (!((Data::getDataValue("imageDirectory","project") == ""))){

        // Find the recordTypePrefix
        std::vector<std::string> recordTypePrefixes = Data::getDictionaryKeysByType("recordType");

        for (const auto& retrievedRecTypePrefix : recordTypePrefixes){

            // Test if the recordType is correct
            if(Data::getDataValue(retrievedRecTypePrefix,"recordType") == recordType){

                returnVal = Data::getDataValue("imageDirectory","project") + "/" +
                            retrievedRecTypePrefix + ".png";
            }
        }
    }

    return returnVal;
}

void ConnectionsAndIntegrations::getRecord(std::string ID) {

    // define the dictionary data.
    DataDictionary ConnectionDataDictionary;

    std::string recordDirectory = Data::getDataValue("connectionDirectory","project");

    // Create the record object
    basicRecord ConnectionRecord(recordDirectory);
    // Get the dataDictionary and update the main dictionary
    ConnectionDataDictionary = ConnectionRecord.getData(ID);

    // Set context of dictionary, and merge it into the main
    Data::mergeDataDictionaryIntoMaster(Data::setContexttoValue(ConnectionDataDictionary,"connection"));

    // Set the Data::dictionary values
    std::string recordFileName = recordDirectory + "/" + ID + ".xml";
    Data::setDataValue("fileName","connection",recordFileName);
}

std::string ConnectionsAndIntegrations::create(const std::string type,
                                               const std::string ToolName,
                                               const std::string ModelOrVersion,
                                               const std::string Key) {

    // Create the record file parsing object
    std::string recordDirectory = Data::getDataValue("connectionDirectory","project");
    if(recordDirectory.empty()){
        std::cerr << "ConnectionsAndIntegrations::create: "
                     "connectionDirectory is not set and could not be inferred.\n";
        return "";
    }

    basicRecord parseFile(recordDirectory);

    // Transfer the data dictionary for the type
    std::vector<std::string> dictionaryContext = {"connection"};
    std::string newID = parseFile.create(Data::getDictionaryOfContext(dictionaryContext));
    Data::setDataValue("fileName","connection",recordDirectory + "/" + newID + ".xml");

    // Add the new fields
    Data::setDataValue("ID","connection",newID);
    Data::setDataValue("type","connection",type);
    Data::setDataValue("ToolName","connection",ToolName);
    Data::setDataValue("ModelOrVersion","connection",ModelOrVersion);
    Data::setDataValue("Key","connection",Key);
    Data::setDataValue("lastModifiedBy","connection",projectInfoRecord::CurrentUser);

    Data::setDataValue("lastModifiedOn",
                       "connection",
                       Data::time_point_to_string(std::chrono::system_clock::now()));

    // return the new record ID
    return newID;

    std::string testVal = Data::getDataValue("ID","connection");
    testVal = Data::getDataValue("ID","connection");
    testVal = Data::getDataValue("type","connection");
    testVal = Data::getDataValue("fileName","connection");
    testVal = Data::getDataValue("ToolName","connection");
    testVal = Data::getDataValue("ModelOrVersion","connection");
    testVal = Data::getDataValue("Key","connection");
    testVal = Data::getDataValue("lastModifiedBy","connection");
    testVal = Data::getDataValue("lastModifiedOn","connection");

}

void ConnectionsAndIntegrations::clearDictionaryRecordData() {

    // Get all record keys
    std::vector<std::string> recordKeys = Data::getDictionaryKeysByType("connection");

    // Iterate through the keys and clear the data. This clears the dictionary directly
    // to avoid clearing the data in the file
    for (const auto &recordKey : recordKeys){
        Data::dictionary->setFieldValue(recordKey, "connection","");
    }
}

void ConnectionsAndIntegrations::remove(std::string ID) {

    // Lock the record file
    lockRecord(ID);

    Data::setDataValue("deleted","connection","true");
}

// Get information for the specified record.
std::string ConnectionsAndIntegrations::getRecordField(const std::string ID,
                                                       const std::string field){

    // Create requirement File Parsing Object
    std::string filePath = Data::getDataValue("connectionDirectory","project") + "/" + ID + ".xml";
    basicRecord parseFile(filePath);

    // Return information from the record
    return parseFile.getRecordField(ID,field);
}

// Get a list of automations IDs.
std::vector<std::string> ConnectionsAndIntegrations::getListOfConnections() {
    basicRecord parsingRecord(Data::getDataValue("connectionDirectory", "project"));
    std::string testVal = Data::getDataValue("connectionDirectory", "project");
    const auto paths = parsingRecord.getListofRecords();

    std::vector<std::string> names;
    names.reserve(paths.size());

    for (const auto& s : paths) {
        std::filesystem::path p(s);
        names.emplace_back(p.stem().string()); // filename without extension
    }
    return names;
}

bool ConnectionsAndIntegrations::lockRecord(std::string ID) {
    return true;// COMMENTED Configuration::lockRecord();
}

bool ConnectionsAndIntegrations::releaseRecord(std::string ID) {
    return true;// COMMENTED Configuration::lockRecord();
}

void automationRecord::getRecord(std::string ID) {

    // define the dictionary data.
    DataDictionary automationDataDictionary;

    std::string recordDirectory = Data::getDataValue("automationDirectory","project");

    // Create the record object
    basicRecord automationRecord(recordDirectory);
    // Get the dataDictionary and update the main dictionary
    automationDataDictionary = automationRecord.getData(ID);

    // Set context of dictionary, and merge it into the main
    Data::mergeDataDictionaryIntoMaster(Data::setContexttoValue(automationDataDictionary,"automation"));

    // Set the Data::dictionary values
    std::string recordFileName = recordDirectory + "/" + ID + ".xml";
    Data::setDataValue("fileName","automation",recordFileName);
}

std::string automationRecord::create(const std::string recordType,
                                     const std::string title,
                                     const std::string& description) {

    // Create the record file parsing object
    std::string recordDirectory = Data::getDataValue("automationDirectory","project");
    basicRecord parseFile(recordDirectory);

    // Transfer the data dictionary for the type
    std::vector<std::string> dictionaryContext = {"automation"};
    std::string newID = parseFile.create(Data::getDictionaryOfContext(dictionaryContext));

    // Add the new fields
    Data::setDataValue("ID","automation",newID);
    Data::setDataValue("fileName","automation",recordDirectory + "/" + newID + ".xml");
    Data::setDataValue("type","automation",recordType);
    Data::setDataValue("title","automation",title);
    Data::setDataValue("descripton","automation",description);

    // return the new record ID
    return newID;
}

void automationRecord::clearDictionaryRecordData() {

    // Get all record keys
    std::vector<std::string> recordKeys = Data::getDictionaryKeysByType("automation");

    // Iterate through the keys and clear the data. This clears the dictionary directly
    // to avoid clearing the data in the file
    for (const auto &recordKey : recordKeys){
        Data::dictionary->setFieldValue(recordKey, "automation","");
    }
}

void automationRecord::remove(std::string ID) {

    // Lock the record file
    lockRecord(ID);

    Data::setDataValue("deleted","automation","true");
}

// Get information for the specified record.
std::string automationRecord::getRecordField(const std::string ID,
                                             const std::string field){

    // Create requirement File Parsing Object
    std::string filePath = Data::getDataValue("automationDirectory","project") + "/" + ID + ".xml";
    basicRecord parseFile(filePath);

    // Return information from the record
    return parseFile.getRecordField(ID,field);
}

// Get a list of automations IDs.
std::vector<std::string> automationRecord::getListOfAutomations() {
    basicRecord parsingRecord(Data::getDataValue("automationDirectory", "project"));
    const auto paths = parsingRecord.getListofRecords();

    std::vector<std::string> names;
    names.reserve(paths.size());

    for (const auto& s : paths) {
        std::filesystem::path p(s);
        names.emplace_back(p.stem().string()); // filename without extension
    }
    return names;
}

bool automationRecord::lockRecord(std::string ID) {
    return true;// COMMENTED Configuration::lockRecord();
}

bool automationRecord::releaseRecord(std::string ID) {
    return true;// COMMENTED Configuration::lockRecord();
}

void userRecord::getRecord(std::string ID) {

    // define the dictionary data.
    DataDictionary userDataDictionary;

    std::string recordDirectory = Data::getDataValue(this->directory,"project");

    // Create the record object
    basicRecord userRecord(recordDirectory);
    // Get the dataDictionary and update the main dictionary
    userDataDictionary = userRecord.getData(ID);

    // Set context of dictionary, and merge it into the main
    Data::mergeDataDictionaryIntoMaster(Data::setContexttoValue(userDataDictionary,this->context));

    // Set the Data::dictionary values
    std::string recordFileName = recordDirectory + "/" + ID + ".xml";
    Data::setDataValue("fileName",this->context,recordFileName);
    std::string testVal = Data::getDataValue("userName","userInfo");
}

std::string userRecord::create(const std::string userID,
                               const std::string permissions,
                               const std::string& email) {

    // Create the record file parsing object
    std::string recordDirectory = Data::getDataValue(this->directory,"project");
    std::string testVal = Data::getDataValue("userInfo","project");
    testVal = Data::getDataValue("projectDirectory","project");
    testVal = Data::getDataValue("imageDirectory","project");
    basicRecord parseFile(recordDirectory);

    // Transfer the data dictionary for the type
    std::vector<std::string> dictionaryContext = {this->context};
    std::string newID = parseFile.create(Data::getDictionaryOfContext(dictionaryContext));

    // Set the new fields
    Data::setDataValue("ID",this->context,newID);
    Data::setDataValue("fileName",this->context,recordDirectory + "/" + newID + ".xml");
    Data::setDataValue("userName",this->context,userID);
    Data::setDataValue("permissions",this->context,permissions);
    Data::setDataValue("email",this->context,email);

    // Set the creation date
    Data::setDataValue("lastModifiedOn",
                       this->context,
                       Data::time_point_to_string(std::chrono::system_clock::now()));

    // return the new record ID
    return newID;
}

void userRecord::clearDictionaryRecordData() {

    // Get all record keys
    std::vector<std::string> recordKeys = Data::getDictionaryKeysByType(this->context);

    // Iterate through the keys and clear the data. This clears the dictionary directly
    // to avoid clearing the data in the file
    for (const auto &recordKey : recordKeys){
        Data::dictionary->setFieldValue(recordKey,this->context,"");
    }
}

void userRecord::remove(std::string ID) {

    // Lock the record file
    lockRecord(ID);

    Data::setDataValue("deleted",this->context,"true");
}

// Get information for the specified record.
std::string userRecord::getRecordField(const std::string ID,
                                       const std::string field){

    // Create requirement File Parsing Object
    std::string filePath = Data::getDataValue(this->context,"project") + "/" + ID + ".xml";
    basicRecord parseFile(filePath);

    // Return information from the record
    return parseFile.getRecordField(ID,field);
}

// Get a list of IDs.
std::vector<std::string> userRecord::getListOfUsers() {
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

bool userRecord::lockRecord(std::string ID) {
    return true;// COMMENTED Configuration::lockRecord();
}

bool userRecord::releaseRecord(std::string ID) {
    return true;// COMMENTED Configuration::lockRecord();
}

void reviewRecord::getRecord() {

    std::string recordDirectory = Data::getDataValue("reviewDirectory","project");
    std::string recordID = Data::getDataValue("ID","review");

    // Clear the old record data
    reviewRecord::clearDictionaryRecordData();

    // Set the Data::dictionary values
    std::string recordFileName = recordDirectory + "/" + recordID + ".xml";
    Data::setDataValue("fileName","review",recordFileName);

    fileOperations::reviewFileOperations parseFile(recordDirectory);
    // Copy data dictionary
    parseFile.fieldInformationDictionary = *Data::dictionary;
    parseFile.readFile(recordID);
    // Set the filename
    parseFile.fieldInformationDictionary.setFieldValue("fileName","review",recordFileName);
    *Data::dictionary = parseFile.fieldInformationDictionary;
}

std::string reviewRecord::create(const std::string recordType,
                                 const std::string title,
                                 const std::string& description) {

    // Set the ID to date and time in seconds and replace strange characters
    std::string nextID = projectInfoRecord::generateUUIDv4();

    // Create the record file parsing object
    std::string recordDirectory = Data::getDataValue("reviewDirectory","project");
    fileOperations::reviewFileOperations parseReviewFile(recordDirectory);
    // Transfer the dictionary data
    parseReviewFile.fieldInformationDictionary = *Data::dictionary;
    std::string newRecordID = parseReviewFile.createFile(recordDirectory,recordType,nextID);

    // Make sure there is a new record created. If an ID was returned, then
    // the record was correctly created
    if (newRecordID.empty()) {
        return "";
    }

    // Copy the dictionary back
    *Data::dictionary = parseReviewFile.fieldInformationDictionary;
    // Set the version to the first one
    Data::setDataValue("version","review","1");

    // return the new record ID
    return newRecordID;
}

void reviewRecord::edit() {

    // Lock the record in CM
    lockRecord();

    // Create the record file parsing object
    fileOperations::reviewFileOperations parseAutomationFile(Data::getDataValue("reviewDirectory","project"));

    // UPdate the timestamp and user information
    Data::setDataValue("lastModifiedBy", "automation", Data::getDataValue("userID", "project"));
    Data::setDataValue("lastModifiedOn", "automation", Data::time_point_to_string(std::chrono::system_clock::now()));

    parseAutomationFile.editReviewFile();
    // COMMENTED Configuration::commitRecord();
    // COMMENTED Configuration::commitProjectFile();
}

void reviewRecord::clearDictionaryRecordData() {

    // Get all record keys
    std::vector<std::string> recordKeys = Data::getDictionaryKeysByType("review");

    // Iterate through the keys and clear the data. This clears the dictionary directly
    // to avoid clearing the data in the file
    for (const auto &recordKey : recordKeys){
        Data::dictionary->setFieldValue(recordKey, "review","");
    }
}

void reviewRecord::remove(void) {

    // Lock the record file
    lockRecord();

    Data::setDataValue("deleted","review","true");
}

// ---- Review helpers (added to satisfy linker) ----
void reviewRecord::addReviewer(std::string userid, std::string name, std::string role) {
    if (userid.empty()) return;
    Data::addData("reviewer_name_" + userid, "review", name, "true", "Reviewer name");
    Data::addData("reviewer_role_" + userid, "review", role, "true", "Reviewer role");
}

void reviewRecord::removeReviewer(std::string userid) {
    if (userid.empty()) return;
    if (Data::keyExists("reviewer_name_" + userid, "review"))
        Data::removeData("reviewer_name_" + userid, "review");
    if (Data::keyExists("reviewer_role_" + userid, "review"))
        Data::removeData("reviewer_role_" + userid, "review");
}

std::vector<std::string> reviewRecord::getAllReviewersUserIDs(void) {
    std::vector<std::string> ids;
    auto keys = Data::getDictionaryKeysByType("review");
    for (const auto& k : keys) {
        const std::string prefix = "reviewer_name_";
        if (k.rfind(prefix, 0) == 0) {
            ids.push_back(k.substr(prefix.size()));
        }
    }
    return ids;
}

std::string reviewRecord::getReviewerName(std::string userid) {
    if (userid.empty()) return "";
    return Data::getDataValue("reviewer_name_" + userid, "review");
}

std::string reviewRecord::getReviewerParticipation(std::string userid) {
    if (userid.empty()) return "";
    return Data::getDataValue("reviewer_participation_" + userid, "review");
}

// Get information for the specified record.
std::string reviewRecord::getRecordField(const std::string& recordID,
                                         const std::string& field){

    // Create requirement File Parsing Object
    fileOperations::reviewFileOperations parseReviewFile(Data::getDataValue("reviewDirectory","project"));

    // Return information from the record
    return parseReviewFile.getReviewInformation(recordID, field);
}

bool reviewRecord::lockRecord() {
    return true;// COMMENTED Configuration::lockRecord();
}

void reviewRecord::releaseRecord() {
    // COMMENTED Configuration::releaseRecordLock();
}

// Define constructor by passing in the projectFileLocation
basicRecord::basicRecord(std::string recordLocation)
    : recordDirectory(recordLocation){

}

DataDictionary basicRecord::getData(std::string recordID) {

    // Clear the old record data
    basicRecord::clearDictionaryRecordData();

    // Set the Data::dictionary values
    std::string recordFileName = this->recordDirectory + "/" + recordID + ".xml";
    Data::setDataValue("fileName","basic",recordFileName);

    fileOperations::GeneralFileOperations parseFile(recordFileName);
    parseFile.readFile();

    // Set the filename and ID
    parseFile.fieldInformationDictionary.setFieldValue("fileName","basic",recordFileName);
    return parseFile.fieldInformationDictionary;
}

std::string basicRecord::create(DataDictionary data) {

    // Set the ID to date and time in seconds and replace strange characters
    std::string nextID = projectInfoRecord::generateUUIDv4();

    // Create the file manipulation object
    fileOperations::GeneralFileOperations parseFile(this->recordDirectory +
                                                    "/" + nextID +
                                                    ".xml");
    // Transfer the dictionary data
    if(parseFile.createFile(data,true)){
        return nextID;
    }
    else
    {
        return "";
    }
}

void basicRecord::clearDictionaryRecordData() {

    // Get all record keys
    std::vector<std::string> recordKeys = Data::getDictionaryKeysByType("automation");

    // Iterate through the keys and clear the data. This clears the dictionary directly
    // to avoid clearing the data in the file
    for (const auto &recordKey : recordKeys){
        Data::dictionary->setFieldValue(recordKey, "automation","");
    }
}

void basicRecord::remove(const std::string ID) {

    // Lock the record file
    lockRecord();

    Data::setDataValue("deleted","automation","true");
}

// Get information for the specified record.
std::string basicRecord::getRecordField(const std::string recordID,
                                        const std::string fieldName){

    // Create the file object and read the file
    fileOperations::GeneralFileOperations parseFile(this->recordDirectory +
                                                    "/" + recordID + ".xml");
    parseFile.readFile();

    // Return information from the record
    return parseFile.GetFieldValue(fieldName);
}

// Get information for the specified record.
std::string basicRecord::getRecordAttribute(const std::string ID,
                                            const std::string fieldName,
                                            const std::string attributeName){

    // Create requirement File Parsing Object
    std::string filePath = recordDirectory + "/" + ID + ".xml";
    fileOperations::GeneralFileOperations parseFile(filePath);

    // Return information from the record
    return parseFile.GetAttributeValue(fieldName, attributeName);
}


// Get a list of all records
std::vector<std::string> basicRecord::getListofRecords(){

    fileOperations::generalXMLOperations fileOperations;
    return fileOperations.getXmlFiles(recordDirectory);
}

bool basicRecord::lockRecord() {
    return true;// COMMENTED Configuration::lockRecord();
}

void basicRecord::releaseRecord() {
    // COMMENTED Configuration::releaseRecordLock();
}

