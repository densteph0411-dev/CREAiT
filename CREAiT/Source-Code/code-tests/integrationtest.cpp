#include "integrationtest.h"
#include "Source-Code/dataObjects/records.h"
#include <filesystem>
#include <cassert>

std::string baseTestDirectory = "C:/testDirectory";
std::string projectFileName = "C:/testDirectory/CREAiT/project.xml";

// Create a project
// Test Creating a project
bool integrationTest::createProjectTest(void){

    if(std::filesystem::exists(baseTestDirectory)){
        std::cerr << "Delete the test directory "
                 << baseTestDirectory << std::endl;
        return false;
    }

    // Set up the values, set up in the Documents Directory for easy testing
    Data::setDataValue("type","project","svn");
    bool returnVal = projectInfoRecord::createNew(baseTestDirectory,"Testing","svn");

    return returnVal;
}

// Test Closing the project
bool integrationTest::closeProjectTest(void){
    bool passFailResult = true;

    // First make sure the folder exists
    if(!integrationTest::createProjectTest()){
        projectInfoRecord::clearData();
    }

    // Test Credentials were re-initialized
    if(!Data::dictionary->getFieldValue("userID", "credentials").empty() and
        !Data::dictionary->getFieldValue("passWord", "credentials").empty() and
        !Data::dictionary->getFieldValue("projectName", "project").empty() and
        !Data::dictionary->getFieldValue("type", "project").empty()){

        return false;
    }
    return passFailResult;
}

// Test Opening the project
bool integrationTest::openProjectTest(void){

    Data::setDataValue("projectName","project","Testing");

    return projectInfoRecord::load(projectFileName,"Testing");
}

// Test to create another project
bool integrationTest::createManyProjectTests(void){

    // Setting true, then if projects are not created, it gets set false
    bool passFailResult = true;

    // Initialize the projectInfoRecord
    projectInfoRecord::initialize();

    // Set up the values, set up in the Documents Directory for easy testing
    Data::setDataValue("baseDirectory","project",baseTestDirectory);
    Data::setDataValue("type","project","svn");

    std::string testName = "";
    for (int i = 0; i < 50; i++){
        testName = "Testing-" + std::to_string(i);
        Data::setDataValue("projectName","project",testName);

        // Create a project
        if(!projectInfoRecord::createNew(baseTestDirectory,testName,"svn")){
            passFailResult = false;
        }
    }

    return passFailResult;
}

// Test to create another project
bool integrationTest::getRecentProjectsTest(void){
    bool passFailResult = false;

    // Clear any previouse project data
    projectInfoRecord::clearData();

    // Set the project file name
    Data::setDataValue("fileName","project",projectFileName);

    // Get the recent projects
    std::vector<std::string> recentProjectList = projectInfoRecord::getRecentProjects();

    if(recentProjectList.size() == 51){
        passFailResult = true;
    }

    return passFailResult;
}

// Test to create another project
bool integrationTest::changeToAllProjectsTest(void){

    // Set the boolean true. If it fails one time, it's
    // set false
    bool passFailResult = true;

    std::string baseDirectory;
    std::string projectDirectory;
    std::string programDirectory;
    std::string testDirectory;

    // Set the project file name
    Data::setDataValue("fileName","project",projectFileName);

    // Get the recent projects
    std::vector<std::string> recentProjectList = projectInfoRecord::getRecentProjects();

    std::string testName = "";

    if(recentProjectList.size() == 0){
        passFailResult = false;
    }
    else
    {
        for (int i = 0; i < recentProjectList.size(); i++){

            // Clear any previouse project data
            projectInfoRecord::clearData();

            // Load the next project data
            if (projectInfoRecord::load(projectFileName,recentProjectList[i])){

                if(Data::getDataValue("baseDirectory","project").empty() or
                    Data::getDataValue("projectDirectory","project").empty() or
                    Data::getDataValue("programDirectory","project").empty()){
                    return false;
                }

                baseDirectory = Data::getDataValue("baseDirectory","project");
                projectDirectory = Data::getDataValue("projectDirectory","project");
                programDirectory = Data::getDataValue("programDirectory","project");

                // Test all of the directories are loaded as well
                for (const auto& record : projectInfoRecord::recordList){

                    testDirectory = Data::getDataValue(record + "Directory","project");

                    if(Data::getDataValue(record + "Directory","project").empty()){
                        passFailResult = false;
                    }
                }
            }
            else
            {
                passFailResult = false;
            }
        }
    }

    return passFailResult;
}

// Test to create another project
bool integrationTest::createRecordTest(void){
    bool passFailResult = false;

    std::string projectName = Data::getDataValue("projectName","project");
    std::string toolName = Data::getDataValue("toolName","project");
    std::string baseDirectory = Data::getDataValue("baseDirectory","project");
    std::string programDirectory = Data::getDataValue("programDirectory","project");
    std::string projectDirectory = Data::getDataValue("projectDirectory","project");
    std::string recordDirectory = Data::getDataValue("recordDirectory","project");

    // Create one record
    std::string returnedRecordID;
    returnedRecordID = dataRecord::create("System Requirement","System Requirement 1","PROJ","","first item in the index");
    std::string recordPath = recordDirectory + "/" + returnedRecordID + ".xml";

    // Test if the record and file was created
    if(returnedRecordID == "SyREQ_1"){

        // Test the file is present
        if(std::filesystem::exists(recordPath)){
            passFailResult =true;
        }
    }

    // Test the records are present
    std::string programDir = Data::getDataValue("projectDirectory","project");
    std::vector<std::string> recordList = projectInfoRecord::getAllRecords();

    return passFailResult;
}

// Test to create many records
bool integrationTest::createManyRecordsTest(void){

    // Get all record Type mnemonics
    std::vector<std::string> recordTypes = Data::getDictionaryKeysByType("recordType");
    std::string recordTypeName = "";
    std::string newNodeID = "";
    std::string nodePredecessorID = "";
    std::string recordDirectory = "";
    std::string fileName = "";

    // Create one of each record type in each of the directories of the current project
    for (int i = 1; i < 18; i++){

        for (const auto& recordType : recordTypes){

            // Get the recordType name
            recordTypeName = Data::getDataValue(recordType,"recordType");

            // Loads the node ID above as the predicessor
            nodePredecessorID = dataRecord::create(recordTypeName,
                                                   recordTypeName + " " + std::to_string(i),
                                               "DIR_" + std::to_string(i),nodePredecessorID,"");

            Data::setDataValue("ID","record",nodePredecessorID);
            dataRecord::getRecord();

            fileName = Data::getDataValue("fileName","record");
            recordDirectory = Data::getDataValue("recordDirectory","project");

            // Load the record with other data
            Data::setDataValue("description","record","This is the description for " + nodePredecessorID);
            Data::setDataValue("lastModifiedBy","record","User Number " + std::to_string(i));
            std::string dataVal = "field Value " + std::to_string(1);
            Data::addData("field_" + std::to_string(1),"record",dataVal,"true","description for field " + std::to_string(1));
            dataVal = "field Value " + std::to_string(2);
            Data::addData("field_" + std::to_string(2),"record",dataVal,"true","description for field " + std::to_string(2));
            dataVal = "field Value " + std::to_string(3);
            Data::addData("field_" + std::to_string(3),"record",dataVal,"true","description for field " + std::to_string(3));
            dataVal = "field Value " + std::to_string(4);
            Data::addData("field_" + std::to_string(4),"record",dataVal,"true","description for field " + std::to_string(4));
        }
    }

    // create the boolean return value
    bool passFailResult = true;
    std::string debugVal = "";
    std::string testVal = "";
    std::string currentID = "";

    // Create one of each record type in each of the directories of the current project
    for (int i = 1; i < 18; i++){

        // Test if the records were created
        for (const auto& recordType : recordTypes){

            // Get the recordType name
            recordTypeName = Data::getDataValue(recordType,"recordType");

            // load the record
            currentID = recordType + "_" + std::to_string(i);
            Data::setDataValue("ID","record",currentID);
            dataRecord::getRecord();

            fileName = Data::getDataValue("fileName","record");
            recordDirectory = Data::getDataValue("recordDirectory","project");

            // Directories 1-18 are created initially, skip parsing of them. They
            // do not contain the test fields.
            if(((recordTypeName == "Directory") and (i<18)) or (currentID == "SyREQ_1")){
                continue;
            }

            debugVal = Data::getDataValue("description","record");
            testVal = "This is the description for " + currentID;
            if(!(debugVal == testVal)){
                passFailResult = false;
            }

            debugVal = Data::getDataValue("lastModifiedBy","record");

            // Account for system requirement. SyREQ_1 was created so i will be +1 to account for id not matching
            if(recordTypeName == "System Requirement"){
                testVal = "User Number " + std::to_string(i-1);
            }
            else
            {
                testVal = "User Number " + std::to_string(i);
            }


            if(!(debugVal == testVal)){
                passFailResult = false;
            }

            debugVal = Data::getDataValue("field_" + std::to_string(1),"record");
            testVal = "field Value " + std::to_string(1);
            if(!(debugVal == testVal)){
                passFailResult = false;
            }

            debugVal = Data::getDataValue("field_" + std::to_string(2),"record");
            testVal = "field Value " + std::to_string(2);
            if(!(debugVal == testVal)){
                passFailResult = false;
            }

            debugVal = Data::getDataValue("field_" + std::to_string(3),"record");
            testVal = "field Value " + std::to_string(3);
            if(!(debugVal == testVal)){
                passFailResult = false;
            }

            debugVal = Data::getDataValue("field_" + std::to_string(4),"record");
            testVal = "field Value " + std::to_string(4);
            if(!(debugVal == testVal)){
                passFailResult = false;
            }
        }
    }

    return passFailResult;
}

// Remove Record
bool integrationTest::removeRecordsTest(void){
    bool passFailResult = false;

    // Select the record
    std::string currentID = "SyREQ_1";
    Data::setDataValue("ID","record",currentID);
    dataRecord::getRecord();

    // Remove it
    dataRecord::remove();

    // Test the record is deleted
    std::string deletedVal = Data::getDataValue("deleted","record");
    if(deletedVal == "true"){
        passFailResult = true;
    }

    return passFailResult;
}

// Test to create another project
bool integrationTest::addLinksToRecordTest(void){
    bool passFailResult = false;

    // Load a record
    std::string currentID = "SyREQ_2";
    Data::setDataValue("ID","record",currentID);
    dataRecord::getRecord();

    dataRecord::addLink("SwREQ_1",false,"Trace");

    // Reload the file
    currentID = "SyREQ_2";
    Data::setDataValue("ID","record",currentID);
    dataRecord::getRecord();

    // Test if the arrays are empty, if so this failed
    if(dataRecord::linkName.empty() or
       dataRecord::linkDirection.empty() or
       dataRecord::linkType.empty() or
       dataRecord::linkTitle.empty()){

        return false;
    }

    // Test if all of the data was correctly loaded in the system requirement
    if((dataRecord::linkName[0] == "SwREQ_1") and
       (dataRecord::linkDirection[0] == "Down") and
       (dataRecord::linkType[0] == "Trace") and
       (dataRecord::linkTitle[0] == "Software Requirement 1")){

        // Load the destination link file
        currentID = "SwREQ_1";
        Data::setDataValue("ID","record",currentID);
        dataRecord::getRecord();

        // Test if all of the data was correctly loaded in the system requirement
        if((dataRecord::linkName[0] == "SyREQ_2") and
            (dataRecord::linkDirection[0] == "Up") and
            (dataRecord::linkType[0] == "Trace") and
            (dataRecord::linkTitle[0] == "System Requirement 1")){

            passFailResult = true;
        }
    }

    // Return the boolean
    return passFailResult;
}

// Test to create another project
bool integrationTest::removeLinksToRecordTest(void){
    bool passFailResult = false;

    // Load a record
    std::string currentID = "SyREQ_2";
    Data::setDataValue("ID","record",currentID);
    dataRecord::getRecord();

    dataRecord::removeLink("SwREQ_1");

    // Reload the file
    currentID = "SyREQ_2";
    Data::setDataValue("ID","record",currentID);
    dataRecord::getRecord();

    // Test if all of the data was correctly loaded in the system requirement
    if((dataRecord::linkName.empty()) and
        (dataRecord::linkDirection.empty()) and
        (dataRecord::linkType.empty()) and
        (dataRecord::linkTitle.empty())){

        // Load the destination link file
        currentID = "SwREQ_1";
        Data::setDataValue("ID","record",currentID);
        dataRecord::getRecord();

        // Test if all of the data was correctly loaded in the system requirement
        if((dataRecord::linkName.empty()) and
            (dataRecord::linkDirection.empty()) and
            (dataRecord::linkType.empty()) and
            (dataRecord::linkTitle.empty())){

            passFailResult = true;
        }
    }

    // Return the boolean
    return passFailResult;
}

bool integrationTest::createManyLinksTest(void) {
    bool passFailResult = true;

    try {
        // --------------------------
        // PHASE 1: Create the links
        // --------------------------
        for (int i = 1; i < 18; ++i) {
            std::string parentID = "SyREQ_" + std::to_string(i + 1);
            Data::setDataValue("ID", "record", parentID);
            dataRecord::getRecord();

            dataRecord::addLink("SyTC_" + std::to_string(i), false, "Trace");
            dataRecord::addLink("SwREQ_" + std::to_string(i), false, "Trace");
            dataRecord::addLink("HwREQ_" + std::to_string(i), false, "Trace");

            Data::setDataValue("ID", "record", "SyTC_" + std::to_string(i));
            dataRecord::getRecord();
            dataRecord::addLink("SyTP_" + std::to_string(i), false, "Trace");

            Data::setDataValue("ID", "record", "SwREQ_" + std::to_string(i));
            dataRecord::getRecord();
            dataRecord::addLink("SwTC_" + std::to_string(i), false, "Trace");

            Data::setDataValue("ID", "record", "SwTC_" + std::to_string(i));
            dataRecord::getRecord();
            dataRecord::addLink("SwTP_" + std::to_string(i), false, "Trace");

            Data::setDataValue("ID", "record", "SwTP_" + std::to_string(i));
            dataRecord::getRecord();
            dataRecord::addLink("Code_" + std::to_string(i), false, "Trace");

            Data::setDataValue("ID", "record", "HwREQ_" + std::to_string(i));
            dataRecord::getRecord();
            dataRecord::addLink("HwTC_" + std::to_string(i), false, "Trace");

            Data::setDataValue("ID", "record", "HwTC_" + std::to_string(i));
            dataRecord::getRecord();
            dataRecord::addLink("HwTP_" + std::to_string(i), false, "Trace");

            Data::setDataValue("ID", "record", "HwTP_" + std::to_string(i));
            dataRecord::getRecord();
            dataRecord::addLink("DRW_" + std::to_string(i), false, "Trace");
        }

        // --------------------------
        // PHASE 2: Verify the links
        // --------------------------
        for (int i = 1; i < 18; ++i) {
            std::vector<std::pair<std::string, std::string>> expectedLinks = {
                {"SyREQ_" + std::to_string(i + 1), "SyTC_" + std::to_string(i)},
                {"SyREQ_" + std::to_string(i + 1), "SwREQ_" + std::to_string(i)},
                {"SyREQ_" + std::to_string(i + 1), "HwREQ_" + std::to_string(i)},
                {"SyTC_"  + std::to_string(i),     "SyTP_" + std::to_string(i)},
                {"SwREQ_" + std::to_string(i),     "SwTC_" + std::to_string(i)},
                {"SwTC_"  + std::to_string(i),     "SwTP_" + std::to_string(i)},
                {"SwTP_"  + std::to_string(i),     "Code_" + std::to_string(i)},
                {"HwREQ_" + std::to_string(i),     "HwTC_" + std::to_string(i)},
                {"HwTC_"  + std::to_string(i),     "HwTP_" + std::to_string(i)},
                {"HwTP_"  + std::to_string(i),     "DRW_"  + std::to_string(i)}
            };

            for (const auto& [recordID, expectedLink] : expectedLinks) {
                Data::setDataValue("ID", "record", recordID);
                dataRecord::getRecord(); // This will populate dataRecord::linkName and others

                bool found = false;
                for (size_t j = 0; j < dataRecord::linkName.size(); ++j) {
                    if (dataRecord::linkName[j] == expectedLink &&
                        dataRecord::linkType[j] == "Trace") {
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    std::cerr << "[FAIL] " << recordID << " missing link to " << expectedLink << " [type=Trace]" << std::endl;
                    passFailResult = false;
                }
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "[ERROR] createManyLinksTest - Exception: " << e.what() << std::endl;
        passFailResult = false;
    }

    return passFailResult;
}



bool getRecordTypes (){

    bool returnVal = false;

    std::vector<std::string> recordTypes = Data::getDictionaryKeysByType("recordType");

    if(!recordTypes.empty()){
        returnVal = true;
    }

    return returnVal;
}

// Test to create many records
bool integrationTest::createManyAutomationsTest(void){

    // Define the return result. It gets set false if any of the one conditions is not met
    bool passFailResult = true;

    // Get all record Type mnemonics
    std::vector<std::string> recordTypes;
    std::string recordTypeName = "";
    std::string newNodeID = "";
    std::string nodePredecessorID = "";
    std::string recordDirectory = "";
    std::string fileName = "";

    // Create a type list
    recordTypes.push_back("automation");
    recordTypes.push_back("training");

    // create the array to store IDs for testing later
    std::vector<std::string> createdAutomations;

    // Create the automation object
    automationRecord automationRecord;

    // Define the array holding the created IDs and data
    std::vector<std::pair<std::string, std::vector<std::string>>> TestArray;
    std::vector<std::string> testValues;

    // Create one of each record type in each of the directories of the current project
    for (int i = 1; i < 18; i++){

        for (const auto& recordTypeName : recordTypes){

            // Loads the node ID above as the predicessor
            nodePredecessorID = automationRecord.create(recordTypeName,
                                                   recordTypeName + " " + std::to_string(i),
                                                   recordTypeName + " Description " + std::to_string(i));

            // Add ID to the array for testing later
            createdAutomations.push_back(nodePredecessorID);
            Data::setDataValue("ID","automation",nodePredecessorID);

            // Set test values as {description,lastModifiedBy,field_1,field_2,field_3,field_4}
            testValues = {
                          "This is the description for " + nodePredecessorID,
                          "User Number " + std::to_string(i),
                          "field Value 1",
                          "field Value 2",
                          "field Value 3",
                          "field Value 4"};
            TestArray.emplace_back(nodePredecessorID, std::move(testValues));

            automationRecord.getRecord(nodePredecessorID);
            fileName = Data::getDataValue("fileName","automation");
            recordDirectory = Data::getDataValue("automationDirectory","project");

            // Load the record with other data
            Data::setDataValue("description","automation","This is the description for " + nodePredecessorID);
            Data::setDataValue("lastModifiedBy","automation","User Number " + std::to_string(i));
            std::string dataVal = "field Value " + std::to_string(1);
            Data::addData("field_" + std::to_string(1),"automation",dataVal,"true","description for field " + std::to_string(1));
            dataVal = "field Value " + std::to_string(2);
            Data::addData("field_" + std::to_string(2),"automation",dataVal,"true","description for field " + std::to_string(2));
            dataVal = "field Value " + std::to_string(3);
            Data::addData("field_" + std::to_string(3),"automation",dataVal,"true","description for field " + std::to_string(3));
            dataVal = "field Value " + std::to_string(4);
            Data::addData("field_" + std::to_string(4),"automation",dataVal,"true","description for field " + std::to_string(4));
        }
    }

    // Define all of the testing variables
    std::string programDir = Data::getDataValue("projectDirectory","project");;
    std::string title = "";
    std::string type = "";
    std::string version = "";
    std::string Deleted = "";
    std::string testValue = "";

    // Get the automation list
    std::vector<std::string> recordList = automationRecord.getListOfAutomations();

    // Create one of each record type in each of the directories of the current project
    for (const auto& [id, values] : TestArray) {
        std::cout << "ID: " << id << "\n";

        // Get the current record
        automationRecord.getRecord(id);
        testValue = Data::getDataValue("description","automation");
        // Test the new record against the expected values
        if(values[0] != testValue){
            passFailResult = false;
        }
        // Test the new record against the expected values
        testValue = Data::getDataValue("lastModifiedBy","automation");
        if(values[1] != testValue){
                passFailResult = false;
        }
        // Test the new record against the expected values
        testValue = Data::getDataValue("field_1","automation");
        if(values[2] != testValue){
            passFailResult = false;
        }
        // Test the new record against the expected values
        if(values[3] != Data::getDataValue("field_2","automation")){
            passFailResult = false;
        }
        // Test the new record against the expected values
        if(values[4] != Data::getDataValue("field_3","automation")){
            passFailResult = false;
        }
        // Test the new record against the expected values
        if(values[5] != Data::getDataValue("field_4","automation")){
            passFailResult = false;
        }

        title = Data::getDataValue("title", "automation");
        type = Data::getDataValue("type", "automation");
        version = Data::getDataValue("version", "automation");
        Deleted = Data::getDataValue("deleted", "automation");

        for (const auto& val : values) {
            std::cout << "  Value: " << val << "\n";
        }
    }
    return passFailResult;
}

// Test to create many records
bool integrationTest::createManyReviewsTest(void){

   std::vector<std::string> recordTypes = {"reviews"};
   std::vector<std::string> createdRecordIDs;

   // ---------- Step 1: Create and store record IDs ----------
   for (int i = 1; i < 18; i++) {
       for (const auto& recordTypeName : recordTypes) {
           std::string title = recordTypeName + " " + std::to_string(i);
           std::string description = recordTypeName + " Description " + std::to_string(i);

           std::string recordID = reviewRecord::create(recordTypeName, title, description);
           createdRecordIDs.push_back(recordID);

           Data::setDataValue("ID", "review", recordID);
           reviewRecord::getRecord();

           Data::setDataValue("title", "review", recordID);
           Data::setDataValue("type", "review", "review");
           Data::setDataValue("deleted", "review", "false");
           Data::setDataValue("description", "review", "This is the description for " + recordID);
           Data::setDataValue("lastModifiedBy", "review", "User Number " + std::to_string(i));

           for (int j = 1; j <= 4; j++) {
               std::string fieldName = "field_" + std::to_string(j);
               std::string fieldValue = "field Value " + std::to_string(j);
               std::string fieldDesc = "review for field " + std::to_string(j);
               Data::addData(fieldName, "review", fieldValue, "true", fieldDesc);
           }
       }
   }

   // ---------- Step 2: Reopen and validate all records ----------
   for (const std::string& recordID : createdRecordIDs) {
       Data::setDataValue("ID", "review", recordID);
       reviewRecord::getRecord();

       std::string desc = Data::getDataValue("description", "review");
       std::string modBy = Data::getDataValue("lastModifiedBy", "review");

       if (desc.empty() || modBy.empty()) {
           std::cerr << "[FAIL] Record " << recordID << " is missing metadata.\n";
           return false;
       }

       for (int j = 1; j <= 4; j++) {
           std::string fieldName = "field_" + std::to_string(j);
           std::string expected = "field Value " + std::to_string(j);
           std::string actual = Data::getDataValue(fieldName, "review");

           if (actual != expected) {
               std::cerr << "[FAIL] Record " << recordID << ": " << fieldName
                         << " expected '" << expected << "' but got '" << actual << "'\n";
               return false;
           }
       }
   }

   // ---------- Step 3: Validate final state ----------
   std::string title    = Data::getDataValue("title", "review");
   std::string type     = Data::getDataValue("type", "review");
   std::string version  = Data::getDataValue("version", "review");
   std::string deleted  = Data::getDataValue("deleted", "review");
   std::string projDir  = Data::getDataValue("projectDirectory", "project");

   if (title.empty() || type.empty() || version.empty() || deleted.empty() || projDir.empty()) {
       std::cerr << "[FAIL] Missing final review or project info.\n";
       return false;
   }

   std::vector<std::string> reviews = projectInfoRecord::getAllReviews();
   if (reviews.empty()) {
       std::cerr << "[FAIL] No reviews found in projectInfoRecord.\n";
       return false;
   }

   std::cout << "[PASS] All records created, reopened, and validated successfully.\n";
   return true;
}

bool integrationTest::testRecordTypePaths() {
    bool returnVal = true;

    std::string imagePath = "";
    std::vector<std::string> recordTypePrefixes = Data::getDictionaryKeysByType("recordType");

    for (const auto& retrievedRecTypePrefix : recordTypePrefixes) {
        imagePath = Data::getDataValue("imageDirectory", "project") + "/" +
                    retrievedRecTypePrefix + ".png";

        if (!(imagePath == projectInfoRecord::getRecordTypeImageFilePath(
                  Data::getDataValue(retrievedRecTypePrefix, "recordType")))) {
            returnVal = false;
        }
    }

    return returnVal;
}

// Test to create many records
bool integrationTest::createIntegrations(){

    // Define the return result. It gets set false if any of the one conditions is not met
    bool passFailResult = true;

    // Get all record Type mnemonics
    std::vector<std::string> recordTypes;
    std::string recordTypeName = "";
    std::string newNodeID = "";
    std::string nodePredecessorID = "";
    std::string recordDirectory = "";
    std::string fileName = "";

    // Create a type list
    recordTypes.push_back("AIConnection");
    recordTypes.push_back("integration");

    // create the array to store IDs for testing later
    std::vector<std::string> createdConnections;

    // Create the automation object
    ConnectionsAndIntegrations ConnectionRecord;

    // Define the array holding the created IDs and data
    std::vector<std::pair<std::string, std::vector<std::string>>> TestArray;
    std::vector<std::string> testValues;

    // Create one of each record type in each of the directories of the current project
    for (int i = 1; i < 18; i++){

        for (const auto& recordTypeName : recordTypes){

            // Loads the node ID above as the predicessor
            nodePredecessorID = ConnectionRecord.create(recordTypeName,
                                                        recordTypeName + std::to_string(i),
                                                        "ModelVersion" + std::to_string(i),
                                                        "ABCD34546565");

            // Add ID to the array for testing later
            createdConnections.push_back(nodePredecessorID);
            Data::setDataValue("ID","connection",nodePredecessorID);

            // Set test values as {description,lastModifiedBy,field_1,field_2,field_3,field_4}
            testValues = {recordTypeName,
                          recordTypeName + std::to_string(i),
                          "ModelVersion" + std::to_string(i),
                          "ABCD34546565"};
            TestArray.emplace_back(nodePredecessorID, std::move(testValues));

            ConnectionRecord.getRecord(nodePredecessorID);
            fileName = Data::getDataValue("fileName","connection");
            recordDirectory = Data::getDataValue("connectionDirectory","project");
        }
    }

    // Define all of the testing variables
    std::string programDir = Data::getDataValue("projectDirectory","project");;
    std::string testValue = "";

    // Get the automation list
    std::vector<std::string> recordList = ConnectionRecord.getListOfConnections();

    // Create one of each record type in each of the directories of the current project
    for (const auto& [id, values] : TestArray) {
        std::cout << "ID: " << id << "\n";

        // Get the current record
        ConnectionRecord.getRecord(id);
        testValue = Data::getDataValue("type","connection");
        // Test the new record against the expected values
        if(values[0] != testValue){
            passFailResult = false;
        }
        // Test the new record against the expected values
        testValue = Data::getDataValue("ToolName","connection");
        if(values[1] != testValue){
            passFailResult = false;
        }
        // Test the new record against the expected values
        testValue = Data::getDataValue("ModelOrVersion","connection");
        if(values[2] != testValue){
            passFailResult = false;
        }
        // Test the new record against the expected values
        if(values[3] != Data::getDataValue("Key","connection")){
            passFailResult = false;
        }

        for (const auto& val : values) {
            std::cout << "  Value: " << val << "\n";
        }
    }
    return passFailResult;
}

// Create document data
bool createDocData(void){

    bool returnVal = true;

    // Create the record object
    basicRecord DocRecord(Data::getDataValue("documentDirectory","project"));
    std::vector<std::string> dictionaryContext = {"documents"};
    DataDictionary fieldDictionary = Data::getDictionaryOfContext(dictionaryContext);

    // ---------- Step 1: Create and store record IDs ----------
    for (int i = 1; i < 18; i++) {
        DocRecord.create(fieldDictionary);
    }

    integrationTest test;

    // ---------- Step 1: Create and store record IDs ----------
    for (int i = 1; i < 18; i++) {

        if(!test.createDocTest("document_" + std::to_string(i),
                                "Title "+ std::to_string(i),
                                "Description for " + std::to_string(i))){
            returnVal = false;
        }
    }

    return returnVal;
}

bool integrationTest::createDocTest(std::string ID, std::string title, std::string description){

    return true;
}

// Test to create many records
bool integrationTest::createUserData(){

    // Define the return result. It gets set false if any of the one conditions is not met
    bool passFailResult = true;

    // Get all record Type mnemonics
     std::vector<std::string> recordTypes;
    std::string recordTypeName = "userInfo";
    std::string newNodeID = "";
    std::string nodePredecessorID = "";
    std::string recordDirectory = "";
    std::string fileName = "";

    // create the array to store IDs for testing later
    std::vector<std::string> createdUsers;

    // Create the automation object
    userRecord UserRecord;

    // Define the array holding the created IDs and data
    std::vector<std::pair<std::string, std::vector<std::string>>> TestArray;
    std::vector<std::string> testValues;

    // Create one of each record type in each of the directories of the current project
    for (int i = 1; i < 18; i++){

        // Set up values to be created
        testValues.push_back("UserNumber" + std::to_string(i));
        testValues.push_back("admin");
        testValues.push_back(std::to_string(i) + "@systaero.com");

        // Loads the node ID above as the predicessor
        nodePredecessorID = UserRecord.create(testValues[0],
                                              testValues[1],
                                              testValues[2]);

        // Add ID to the array for testing later
        createdUsers.push_back(nodePredecessorID);
        Data::setDataValue("ID","userInfo",nodePredecessorID);

        // load the record just created
        UserRecord.getRecord(nodePredecessorID);
        Data::setDataValue("userName","userInfo",testValues[0]);
        Data::setDataValue("permissions","userInfo","developer");
        testValues[1] = "developer";
        Data::setDataValue("email","userInfo",testValues[2]);

        // Add the set values to the test array
        TestArray.emplace_back(nodePredecessorID, std::move(testValues));
    }

    // Define all of the testing variables
    std::string testValue = "";

    // Get the automation list
    std::vector<std::string> userList = UserRecord.getListOfUsers();

    // Create one of each record type in each of the directories of the current project
    for (const auto& [id, values] : TestArray) {
        std::cout << "ID: " << id << "\n";

        // Get the current record
        UserRecord.getRecord(id);
        testValue = Data::getDataValue("userName","userInfo");
        // Test the new record against the expected values
        if(values[0] != testValue){
            passFailResult = false;
        }
        // Test the new record against the expected values
        testValue = Data::getDataValue("permissions","userInfo");
        if(values[1] != testValue){
            passFailResult = false;
        }
        // Test the new record against the expected values
        testValue = Data::getDataValue("email","userInfo");
        if(values[2] != testValue){
            passFailResult = false;
        }

        for (const auto& val : values) {
            std::cout << "  Value: " << val << "\n";
        }
    }
    return passFailResult;
}

