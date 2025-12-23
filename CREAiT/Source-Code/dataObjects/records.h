//---------------------------------------------------------------------------
// $Rev: 507 $: Revision of last commit
// $Author: evanmoyle $ Author of last commit
// $Date: 2025-12-18 09:34:19 -0700 (Thu, 18 Dec 2025) $ Date of last commit
// $HeadURL: svn://75.70.242.166/requirementTool/Software/CREAiT/CREAiT-Qt-GUI/CREAiT/Source-Code/dataObjects/records.h $ URL Location
// Copyright 2023-2025
//---------------------------------------------------------------------------
#pragma once

#include "dataDictionary.h"
#include <chrono>
#include <vector>
#include <records.h>

// Data class holds a dictionary of record fields.
class Data {
public:
    Data() {
        initializeData();
    }
    ~Data() {
        // If dictionary is allocated dynamically, clean it up here.
        // For example:
        // delete dictionary;
        // (Alternatively, consider using smart pointers.)
    }
    
    // Create a dictionary object to hold the record, project, credential etc..
    // (Assuming dataDictionary is a standard C++ class defined elsewhere.)
    static DataDictionary* dictionary;

    // Add a field to the record.
    static void addData(const std::string fieldName,
                        const std::string context,
                        const std::string fieldValue,
                        const std::string persistence,
                        const std::string description);

    // Test if the value is in a record, or a project file.
    // Records are in a directory named context within the project directory, and record files
    // are named by context and reside in the progam directory.
    // Returns true if it's a record
    static bool isAProjectFile(std::string fileName);

    // Set the field value for the specified field and context.
    static void setDataValue(const std::string fieldName,
                             const std::string context,
                             const std::string fieldValue);
    
    // Remove a field from the record.
    static void removeData(const std::string fieldKey,
                           const std::string context);
    
    // Get the field value for the specified field.
    static std::string getDataValue(const std::string fieldName,
                                    const std::string context);

    // Get the field persistence type for the specified field.
    static std::string getDataPersistence(const std::string key,
                                          const std::string context);
    
    // Get the field description for the specified field.
    static std::string getDataDescription(const std::string key,
                                          const std::string context);
    
    // Clear the data dictionary by context.
    static void clearDataDictionaryByContext(const std::string context);
    
    // Get an array (vector) of keys for the specified type.
    static std::vector<std::string> getDictionaryKeysByType(const std::string context);

    // Returns an array of all contexts in the data dictionary
    static std::vector<std::string> getAllContexts(void);

    // Returns a dictionary with all keys in the provided context that exist
    static DataDictionary getDictionaryOfContext(const std::vector<std::string> contexts);

    // Initialization routine.
    void initializeData() {
        // Any initialization code if needed.
    }
    
    // Test if a key exists in the dictionary.
    static bool keyExists(const std::string key, const std::string context);

    static std::string time_point_to_string(const std::chrono::system_clock::time_point& time_point,
                                            const std::string& format= "%Y-%m-%d %H:%M:%S");

    // parses the Data dictionary data, and merges it into the main dictionary.
    // If the fields already exist, they will be updated with the new values in data, if not
    // they will be created.
    static void mergeDataDictionaryIntoMaster(DataDictionary data);

    // Changes the context of all keys in the provided dictionary to the value provided and returns the
    // updated dictionary
    // This is used when creating a basic dictionary and merging into the main datastore
    static DataDictionary setContexttoValue (DataDictionary data, std::string context);
};

// dataRecord holds the record information.
class dataRecord {
public:
    // links for the loaded record
    static std::vector<std::string> linkName;
    static std::vector<std::string> linkDirection;
    static std::vector<std::string> linkType;
    static std::vector<std::string> linkTitle;

    static std::vector<std::string> versionNumber;
    static std::vector<std::string> versionDescription;
    static std::vector<std::string> versionDate;

    // Temporary variable used for linking target.
    static std::string targetLinkID;

    // Updates the record values.
    static void getRecord();

    // Create a new record and return the new record ID.
    // A project must be loaded to create a record. Additionally
    // Specify the type, title, and description if know.
    static std::string create(const std::string recordType,
                              const std::string title,
                              const std::string parent,
                              const std::string index,
                              const std::string& description);

    // Edit an existing record.
    static void edit();

    // Clear the record information stored in the static buffer.
    static void clearDictionaryRecordData();

    // Delete the record.
    static void remove();

    // Add links to the record.
    static void addLink(std::string reqID,
                        bool linkIsUp,
                        std::string linkType);

    // Remove links from the record.
    static void removeLink(const std::string& reqID);

    // Get information for the specified record.
    static std::string getRecordField(const std::string& recordID,
                                     const std::string& field);

    // Lock the record.
    static bool lockRecord();

    // Release the record.
    static void releaseRecord();
};

// projectInfoRecord holds project-related information.
class projectInfoRecord {
public:

    // Defines the record Types
    inline static const std::vector<std::string> recordList = {"record",
                                                               "image",
                                                               "automation",
                                                               "review",
                                                               "connection",
                                                               "user",
                                                               "document"};

    // Defines the project data stores
    inline static const std::vector<std::string> projectFileList = {"project",
                                                                    "credentials",
                                                                    "linkType",
                                                                    "recordType"};

    // Define the current user variable. This is used throughout the application
    inline static std::string CurrentUser = "emoyle";
    inline static bool DocumentsEnabled = true;
    inline static bool AutomationsEnabled = true;
    inline static bool ReviewsEnabled = true;

    // Updates the project information.
    static bool load(std::string projectFileName, std::string projectName);

    // Create a new project.
    static bool createNew(std::string baseDirectory,
                          std::string projectName,
                          std::string type);

    // Clear the project information stored in the static buffer.
    static void clearData();

    // Change the active project.
    static void changeProject();

    // Initialize the project information.
    static void initialize();

    // Get the home directory for Windows, Unix or Linux systems
    static std::string getHomeDirectory();

    // Get a vector of all recent projects
    static std::vector<std::string> getRecentProjects();

    // Set up inital dictionary values for directory paths based on the project file path provided and
    // project name.
    static void setUpDirectoryAndFileValues(std::string projectFileName, std::string projectName);

    // Removes stringToRemove from inputString and returns the result
    static std::string removeString(std::string inputString, std::string stringToRemove);

    // Generates a unique ID using UUID (Universally Unique Identifier)
    static std::string generateUUIDv4(void);

    // Get the image path for the record Type
    static std::string getRecordTypeImageFilePath(std::string recordType);

    // Returns an array of all records in the records folder that have not been deleted
    static std::vector<std::string> getAllRecords(void);

    // Returns an array of all automations in the automations folder
    static std::vector<std::string> getAllAutomations(void);

    // Returns an array of all reviewss in the automations folder
    static std::vector<std::string> getAllReviews(void);

    // Returns an array of all integrations in the project
    static std::vector<std::string> getAllIntegrations(void);
};

class ConnectionsAndIntegrations {
public:

    ~ConnectionsAndIntegrations() = default;

    // Updates the record values.
    void getRecord(std::string ID);

    // Create a new record and return the new record ID.
    // A project must be loaded to create a record. Additionally
    // Specify the type, title, and description if know.
    std::string create(const std::string type,
                       const std::string ToolName,
                       const std::string ModelOrVersion,
                       const std::string Key);

    // Clear the record information stored in the static buffer.
    void clearDictionaryRecordData();

    // Delete the record.
    void remove(std::string ID);

    // Get information for the specified record.
    std::string getRecordField(const std::string ID,
                               const std::string field);

    // Get automation list
    std::vector<std::string> getListOfConnections(void);

    // Lock the record.
    bool lockRecord(std::string ID);

    // Release the record.
    bool releaseRecord(std::string ID);

    // Initialize the data record.
    void initialize(std::string ID);

};

// dataRecord holds the record information.
class automationRecord {
public:

    // Updates the record values.
    void getRecord(std::string ID);

    // Create a new record and return the new record ID.
    // A project must be loaded to create a record. Additionally
    // Specify the type, title, and description if know.
    std::string create(const std::string recordType = "",
                              const std::string title = "",
                              const std::string& description = "");

    // Clear the record information stored in the static buffer.
    void clearDictionaryRecordData();

    // Delete the record.
    void remove(std::string ID);

    // Get information for the specified record.
    std::string getRecordField(const std::string ID,
                               const std::string field);

    // Get automation list
    std::vector<std::string> getListOfAutomations(void);

    // Lock the record.
    bool lockRecord(std::string ID);

    // Release the record.
    bool releaseRecord(std::string ID);

    // Initialize the data record.
    void initialize(std::string ID);
};

// dataRecord holds the record information.
class userRecord {
public:

    // Updates the record values.
    void getRecord(std::string ID);

    // Create a new record and return the new record ID.
    // A project must be loaded to create a record. Additionally
    // Specify the type, title, and description if know.
    std::string create(const std::string recordType = "",
                       const std::string title = "",
                       const std::string& description = "");

    // Clear the record information stored in the static buffer.
    void clearDictionaryRecordData();

    // Delete the record.
    void remove(std::string ID);

    // Get information for the specified record.
    std::string getRecordField(const std::string ID,
                               const std::string field);

    // Get the user list
    std::vector<std::string> getListOfUsers(void);

    // Lock the record.
    bool lockRecord(std::string ID);

    // Release the record.
    bool releaseRecord(std::string ID);

    // Initialize the data record.
    void initialize(std::string ID);

private:
    std::string context = "userInfo";
    std::string directory = "userDirectory";
};

// dataRecord holds the record information.
class reviewRecord {
public:

    // Temporary variable used for linking target.
    static std::string targetLinkID;

    // Updates the record values.
    static void getRecord();

    // Create a new record and return the new record ID.
    // A project must be loaded to create a record. Additionally
    // Specify the type, title, and description if know.
    static std::string create(const std::string recordType = "",
                              const std::string title = "",
                              const std::string& description = "");

    // Edit an existing record.
    static void edit();

    // Clear the record information stored in the static buffer.
    static void clearDictionaryRecordData();

    // Delete the record.
    static void remove();

    // Get information for the specified record.
    static std::string getRecordField(const std::string& recordID,
                                      const std::string& field);

    // Lock the record.
    static bool lockRecord();

    // Release the record.
    static void releaseRecord();

    // Add reviewer Info
    static void addReviewer(std::string userid, std::string name, std::string role);

    // Remove reviewer
    static void removeReviewer(std::string userid);

    // Get a list of all reviewers userid's
    static std::vector<std::string> getAllReviewersUserIDs();

    // Get the reviewer's Name
    static std::string getReviewerName(std::string userid);

    // Get a the reviewer's Particpation Status
    static std::string getReviewerParticipation(std::string userid);

    //
};

// basicRecord class holds the record information.
class basicRecord {
public:

    std::string recordDirectory;

    basicRecord(std::string recordLocation);

    // Retrieves data values.
    DataDictionary getData(std::string ID);

    // Create a new record and return the new record ID.
    // A project must be loaded to create a record. Additionally
    // Specify the type, title, and description if know.
    std::string create(DataDictionary data);

    // Clear the record information stored in the static buffer.
    void clearDictionaryRecordData();

    // Set the record to deleted.
    void remove(std::string ID);

    // Get information for the specified record.
    std::string getRecordField(const std::string ID,
                               const std::string fieldName);

    // Get information for the specified record.
    std::string getRecordAttribute(const std::string ID,
                                   const std::string fieldName,
                                   const std::string attributeName);

    // Initializes the record
    std::string initializeRecord();

    // Get a list of all records
    std::vector<std::string> getListofRecords();

    // Lock the record.
    bool lockRecord();

    // Release the record.
    void releaseRecord();
};




