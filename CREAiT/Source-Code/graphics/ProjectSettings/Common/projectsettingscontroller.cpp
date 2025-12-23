#include "projectsettingscontroller.h"
#include "records.h"
#include <QDebug>

// Initialize static members

DatabasesData ProjectSettingsController::databasesData;

QStringList ProjectSettingsController::additionalFields = {
    "Environment=Production",
    "OS=Windows 10",
    "License=Enterprise"
};

// New ID counters
int ProjectSettingsController::integrationIdCounter = 20;
int ProjectSettingsController::aiConnectionIdCounter = 20;
int ProjectSettingsController::userIdCounter = 20;

// ========== Integration CRUD ==========
QMap<int, IntegrationData> ProjectSettingsController::integrations;

void ProjectSettingsController::addIntegration(const IntegrationData& data) {
    qDebug() << "ProjectSettingsController::addIntegration - Starting";
    qDebug() << "  URL:" << data.url;
    qDebug() << "  Tool Index:" << data.toolDisplayIndex;
    qDebug() << "  Key:" << data.Key;
    qDebug() << "  Username:" << data.username;
    
    // Map toolDisplayIndex to tool name (matches dialog: JAMA=0, Polarion=1, DOORs=2)
    QString toolName;
    QString version = data.version.isEmpty() ? "1.0.0" : data.version;
    switch (data.toolDisplayIndex) {
        case 0: toolName = "JAMA"; break;
        case 1: toolName = "Polarion"; break;
        case 2: toolName = "DOORs"; break;
        default: toolName = "Unknown"; break;
    }
    
    qDebug() << "  Tool Name:" << toolName;
    
    // Clear dictionary data first
    ConnectionsAndIntegrations connInt;
    connInt.clearDictionaryRecordData();
    
    // Set all fields in Data dictionary BEFORE calling create()
    // The create() method reads from Data dictionary to build the XML file
    Data::setDataValue("type", "connection", "Integration");
    Data::setDataValue("ToolName", "connection", toolName.toStdString());
    Data::setDataValue("ModelOrVersion", "connection", version.toStdString());
    Data::setDataValue("Key", "connection", data.Key.toStdString());
    Data::setDataValue("url", "connection", data.url.toStdString());
    Data::setDataValue("userName", "connection", data.username.toStdString());
    Data::setDataValue("password", "connection", data.password.toStdString());
    
    // Set connection date
    std::string connectionDate = Data::time_point_to_string(std::chrono::system_clock::now());
    Data::setDataValue("connectionDate", "connection", connectionDate);
    
    // Set last modified info
    Data::setDataValue("lastModifiedBy", "connection", projectInfoRecord::CurrentUser);
    Data::setDataValue("lastModifiedOn", "connection", connectionDate);
    
    // Call backend create() - this will create the file with all Data dictionary values
    std::string newId = connInt.create("Integration", 
                                        toolName.toStdString(),
                                        version.toStdString(),
                                        data.Key.toStdString());
    
    qDebug() << "ProjectSettingsController::addIntegration - Created with ID:" << QString::fromStdString(newId);
    
    // Add to in-memory map
    IntegrationData newData = data;
    newData.id = QString::fromStdString(newId);
    newData.title = toolName;
    newData.version = version;
    integrations[integrationIdCounter++] = newData;
    
    qDebug() << "ProjectSettingsController::addIntegration - Added to map with key:" << (integrationIdCounter - 1);
}

void ProjectSettingsController::updateIntegration(int id, const IntegrationData& data) {
    if (integrations.contains(id))
        integrations[id] = data;
}

void ProjectSettingsController::deleteIntegrationById(int id) {
    integrations.remove(id);
}

IntegrationData ProjectSettingsController::getIntegrationById(int id) {
    return integrations[id];
}

QMap<int, IntegrationData> ProjectSettingsController::getAllIntegrations() {
    // Clear existing data before repopulating
    integrations.clear();

    IntegrationData newIntegrationData;
    ConnectionsAndIntegrations ConnectionsAndIntegrations;

    // Get list of all connections/integrations
    std::vector<std::string> connectionList = ConnectionsAndIntegrations.getListOfConnections();
    int key = 0;

    for (const auto &connectionId : connectionList) {
        // Load the connection/integration record
        ConnectionsAndIntegrations.getRecord(connectionId);

        // Check if this is an Integration (not AIConnection)
        if (Data::getDataValue("type", "connection") == "Integration") {
            newIntegrationData.toolDisplayIndex = 0; // TODO: get from data if available
            newIntegrationData.id = QString::fromStdString(connectionId);
            newIntegrationData.version = QString::fromStdString(Data::getDataValue("ModelOrVersion", "connection"));
            newIntegrationData.title = QString::fromStdString(Data::getDataValue("ToolName", "connection"));
            newIntegrationData.connectionDate = QString::fromStdString(Data::getDataValue("connectionDate", "connection"));
            newIntegrationData.url = QString::fromStdString(Data::getDataValue("url", "connection"));
            newIntegrationData.Key = QString::fromStdString(Data::getDataValue("Key", "connection"));
            newIntegrationData.username = QString::fromStdString(Data::getDataValue("userName", "connection"));
            newIntegrationData.password = QString::fromStdString(Data::getDataValue("password", "connection"));

            // Insert into map
            integrations.insert(key, newIntegrationData);
            key++;
        }
    }

    return integrations;
}

// ========== AI Connection CRUD ==========
QMap<int, AIConnectionData> ProjectSettingsController::aiConnections;
//QMap<int, AIConnectionData> ProjectSettingsController::aiConnections = {
//    { 0,  { "www.ipsum.com",  "OpenAI",      "1.0.1", "sk-oai-093LqwZKdP"         }},
//    { 1,  { "www.ipsum.com",  "Gemini",      "2.4.0", "gm-key-9XP20Lzqj4"         }},
//    { 2,  { "www.ipsum.com",  "GreekSearch", "3.2.1", "grs-92klKZqL-34"          }},
//    { 3,  { "www.ipsum.com",  "OpenAI",      "2.4.0", "sk-opnAI-ZZ901kxp"        }},
//    { 4,  { "www.ipsum.com",  "Gemini",      "3.2.1", "gm-042xLkql12"            }},
//    { 5,  { "www.ipsum.com",  "GreekSearch", "1.0.1", "gr-search-x002qk"         }},
//    { 6,  { "www.ipsum.com",  "OpenAI",      "3.2.1", "sk-open-v99Pq0lz"         }},
//    { 7,  { "www.ipsum.com",  "Gemini",      "1.0.1", "gm-ai-G34klqM"            }},
//    { 8,  { "www.ipsum.com",  "GreekSearch", "2.4.0", "grk-R29pzKk33L"           }},
//    { 9,  { "www.ipsum.com",  "OpenAI",      "2.4.0", "sk-098pLxQaZZx"           }},
//    { 10, { "www.ipsum.com",  "Gemini",      "2.4.0", "gm-4PzQl12xKz"            }},
//    { 11, { "www.ipsum.com",  "GreekSearch", "3.2.1", "grs-kkLpZZ99Q"            }},
//    { 12, { "www.ipsum.com",  "OpenAI",      "1.0.1", "sk-ai-0098Lpz"            }},
//    { 13, { "www.ipsum.com",  "Gemini",      "3.2.1", "gm-connect-Kkl4z9"        }},
//    { 14, { "www.ipsum.com",  "GreekSearch", "2.4.0", "gr-GKZp91xq0"             }},
//    { 15, { "www.ipsum.com",  "OpenAI",      "3.2.1", "sk-x093pqpLzq"            }},
//    { 16, { "www.ipsum.com",  "Gemini",      "1.0.1", "gm-newAI-qzL91"           }},
//    { 17, { "www.ipsum.com",  "GreekSearch", "1.0.1", "grs-link-88KpQl"          }},
//    { 18, { "www.ipsum.com",  "OpenAI",      "2.4.0", "sk-openAIX-K9pqL"         }},
//    { 19, { "www.ipsum.com",  "Gemini",      "3.2.1", "gm-ai-pzLp93k"            }}
//};

void ProjectSettingsController::addAIConnection(const AIConnectionData& data) {
    qDebug() << "ProjectSettingsController::addAIConnection - Starting";
    qDebug() << "  URL:" << data.url;
    qDebug() << "  AI Model:" << data.aiModel;
    qDebug() << "  Version:" << data.version;
    qDebug() << "  Key:" << data.Key;
    
    // Clear dictionary data first
    ConnectionsAndIntegrations connInt;
    connInt.clearDictionaryRecordData();
    
    // Set all fields in Data dictionary BEFORE calling create()
    // The create() method reads from Data dictionary to build the XML file
    Data::setDataValue("type", "connection", "AIConnection");
    Data::setDataValue("ToolName", "connection", data.aiModel.toStdString());
    Data::setDataValue("ModelOrVersion", "connection", data.version.toStdString());
    Data::setDataValue("Key", "connection", data.Key.toStdString());
    Data::setDataValue("url", "connection", data.url.toStdString());
    
    // Set last modified info
    std::string lastModifiedOn = Data::time_point_to_string(std::chrono::system_clock::now());
    Data::setDataValue("lastModifiedBy", "connection", projectInfoRecord::CurrentUser);
    Data::setDataValue("lastModifiedOn", "connection", lastModifiedOn);
    
    // Call backend create() - this will create the file with all Data dictionary values
    std::string newId = connInt.create("AIConnection",
                                      data.aiModel.toStdString(),
                                      data.version.toStdString(),
                                      data.Key.toStdString());
    
    qDebug() << "ProjectSettingsController::addAIConnection - Created with ID:" << QString::fromStdString(newId);
    
    // Add to in-memory map
    aiConnections[aiConnectionIdCounter++] = data;
    
    qDebug() << "ProjectSettingsController::addAIConnection - Added to map with key:" << (aiConnectionIdCounter - 1);
}

void ProjectSettingsController::updateAIConnection(int id, const AIConnectionData& data) {
    if (aiConnections.contains(id))
        aiConnections[id] = data;
}

void ProjectSettingsController::deleteAIConnectionById(int id) {
    aiConnections.remove(id);
}

AIConnectionData ProjectSettingsController::getAIConnectionById(int id) {
    return aiConnections[id];
}

QMap<int, AIConnectionData> ProjectSettingsController::getAllAIConnections() {
    // Clear existing data before repopulating
    aiConnections.clear();

    ConnectionsAndIntegrations ConnectionsAndIntegrations;

    // Get list of all connections/integrations
    std::vector<std::string> connectionList = ConnectionsAndIntegrations.getListOfConnections();
    int key = 0;

    for (const auto &connectionId : connectionList) {
        // Load the connection record
        ConnectionsAndIntegrations.getRecord(connectionId);

        // Check if this is an AIConnection (not Integration)
        if (Data::getDataValue("type", "connection") == "AIConnection") {
            AIConnectionData newConnectionData;
            newConnectionData.ID = QString::fromStdString(connectionId);
            newConnectionData.url = QString::fromStdString(Data::getDataValue("url", "connection"));
            newConnectionData.aiModel = QString::fromStdString(Data::getDataValue("ToolName", "connection"));
            newConnectionData.Key = QString::fromStdString(Data::getDataValue("Key", "connection"));
            newConnectionData.version = QString::fromStdString(Data::getDataValue("ModelOrVersion", "connection"));

            // Insert into map
            aiConnections.insert(key, newConnectionData);
            key++;
        }
    }

    return aiConnections;
}

// ========== User CRUD ==========
QMap<int, UserData> ProjectSettingsController::users;
//QMap<int, UserData> ProjectSettingsController::users = {
//    { 0,  { "#326BRE8", "alice@example",  "passAlice2024",  0, "March 23, 2025" } },
//    { 1,  { "#229JKL9", "bob@example",    "bobSecure!",     1, "February 12, 2024" } },
//    { 2,  { "#772POX3", "carol@example",  "c@rolPass",      0, "April 1, 2025" } },
//    { 3,  { "#991LMN5", "dave@example",   "d4v3!Access",    1, "May 7, 2025" } },
//    { 4,  { "#145QWE6", "eve@example",    "eveSecret!",     0, "March 18, 2025" } },
//    { 5,  { "#893ZXC4", "frank@example",  "frkP@ssword",    1, "June 11, 2025" } },
//    { 6,  { "#007AAA1", "grace@example",  "grace#2024",     0, "July 30, 2025" } },
//    { 7,  { "#556LMQ7", "heidi@example",  "H321!ok",        1, "March 23, 2025" } },
//    { 8,  { "#880VVV2", "ivan@example",   "ivanKEY!x",      0, "January 3, 2025" } },
//    { 9,  { "#300DEF9", "judy@example",   "judyRulez",      1, "September 14, 2024" } },
//    { 10, { "#668TTR4", "karl@example",   "karlSecret",     0, "May 21, 2025" } },
//    { 11, { "#988CCM6", "lisa@example",   "lis@321",        1, "August 9, 2024" } },
//    { 12, { "#111PPG3", "matt@example",   "matty!",         0, "December 25, 2025" } },
//    { 13, { "#444YYU1", "nina@example",   "n1naPa$$",       1, "November 2, 2024" } },
//    { 14, { "#213NNQ2", "oliver@example", "Olv567pass",     0, "March 23, 2025" } },
//    { 15, { "#124TTD5", "peggy@example",  "peggy^_^",       1, "June 5, 2025" } },
//    { 16, { "#932UUI8", "quinn@example",  "qu!nnStrong",    0, "October 31, 2024" } },
//    { 17, { "#384BBM7", "rick@example",   "rick1234",       1, "April 15, 2025" } },
//    { 18, { "#822JJH9", "sara@example",   "s4ra#key",       0, "March 23, 2025" } },
//    { 19, { "#519MKO0", "tom@example",    "t0mZpass!",      1, "May 9, 2025" } }
//};

void ProjectSettingsController::addUser(const UserData& data) {
    qDebug() << "ProjectSettingsController::addUser - Starting";
    qDebug() << "  ID:" << data.ID;
    qDebug() << "  Name:" << data.name;
    qDebug() << "  Role:" << data.role;
    
    // Clear dictionary data first
    userRecord UserRecord;
    UserRecord.clearDictionaryRecordData();
    
    // Set all fields in Data dictionary BEFORE calling create()
    // The create() method reads from Data dictionary to build the XML file
    Data::setDataValue("userName", "userInfo", data.name.toStdString());
    Data::setDataValue("permissions", "userInfo", data.role == 0 ? "Admin" : "General User");
    
    // Generate email from name (UserData doesn't have email field)
    std::string email = data.name.toStdString() + "@example.com";
    Data::setDataValue("email", "userInfo", email);
    
    // Set last modified date
    std::string lastModifiedOn = Data::time_point_to_string(std::chrono::system_clock::now());
    Data::setDataValue("lastModifiedOn", "userInfo", lastModifiedOn);
    
    // Call backend create() - this will create the file with all Data dictionary values
    // Note: userRecord::create takes (userID, permissions, email)
    std::string newId = UserRecord.create(data.name.toStdString(),
                                           data.role == 0 ? "Admin" : "General User",
                                           email);
    
    qDebug() << "ProjectSettingsController::addUser - Created with ID:" << QString::fromStdString(newId);
    
    // Add to in-memory map with the returned ID
    UserData newData = data;
    newData.ID = QString::fromStdString(newId);
    users[userIdCounter++] = newData;
    
    qDebug() << "ProjectSettingsController::addUser - Added to map with key:" << (userIdCounter - 1);
}

void ProjectSettingsController::updateUser(int id, const UserData& data) {
    if (users.contains(id))
        users[id] = data;
}

void ProjectSettingsController::deleteUserById(int id) {
    users.remove(id);
}

UserData ProjectSettingsController::getUserById(int id) {
    return users[id];
}

QMap<int, UserData> ProjectSettingsController::getUsers() {
    // Clear existing data before repopulating
    users.clear();

    UserData newUserData;
    userRecord UserRecord;

    // Get list of all users
    std::vector<std::string> userList = UserRecord.getListOfUsers();
    int key = 0;

    for (const auto &userId : userList) {
        // Load the user record
        UserRecord.getRecord(userId);

        // Use userId directly as the ID (it's the filename without .xml extension)
        // Also try reading from Data dictionary as fallback
        std::string idFromData = Data::getDataValue("ID", "userInfo");
        if (idFromData.empty()) {
            // If ID not in dictionary, use userId (filename) as ID
            newUserData.ID = QString::fromStdString(userId);
        } else {
            newUserData.ID = QString::fromStdString(idFromData);
        }
        
        newUserData.name = QString::fromStdString(Data::getDataValue("userName", "userInfo"));
        newUserData.password = QString::fromStdString(Data::getDataValue("password", "userInfo"));
        newUserData.addedOn = QString::fromStdString(Data::getDataValue("lastModifiedOn", "userInfo"));
        
        // Get role - convert from string to int if needed
        // Check "permissions" first (as set in addUser), then "role" as fallback
        std::string roleStr = Data::getDataValue("permissions", "userInfo");
        if (roleStr.empty()) {
            roleStr = Data::getDataValue("role", "userInfo");
        }
        newUserData.role = (roleStr == "Admin" || roleStr == "0") ? 0 : 1;
        
        qDebug() << "ProjectSettingsController::getUsers - Loaded user:";
        qDebug() << "  ID:" << newUserData.ID;
        qDebug() << "  Name:" << newUserData.name;
        qDebug() << "  Role:" << newUserData.role;

        // Insert into map using the user ID as key (or sequential key)
        users.insert(key, newUserData);
        key++;
    }

    return users;
}

ProjectSettingsController::ProjectSettingsController() {}

BasicInfoData ProjectSettingsController::getBasicInfo() {
    BasicInfoData data;

    std::string projectName = Data::getDataValue("projectName", "project");
    std::string type = Data::getDataValue("type", "project");
    std::string IDPrefix = Data::getDataValue("IDPrefix", "project");
    std::string credentialFileName = Data::getDataValue("fileNameOnly", "credentials");
    std::string description = Data::getDataValue("description", "project");

    data.projectName = QString::fromStdString(projectName);
    data.type = QString::fromStdString(type);
    data.IDPrefix = QString::fromStdString(IDPrefix);
    data.credentialsFileName = QString::fromStdString(credentialFileName);
    data.description = QString::fromStdString(description);

    return data;
}

void ProjectSettingsController::setBasicInfo(const BasicInfoData& data) {
    // basicInfoData = data;
    // Data::setDataValue("projectName", "project", data.projectName.toStdString());
    Data::setDataValue("projectType", "project", data.type.toStdString());
    Data::setDataValue("IDPrefix", "project", data.IDPrefix.toStdString());
    Data::setDataValue("fileNameOnly", "credentials", data.credentialsFileName.toStdString());
    Data::setDataValue("description", "project", data.description.toStdString());
}

DirectoriesData ProjectSettingsController::getDirectories() {
    DirectoriesData data;

    std::string baseDirectory = Data::getDataValue("baseDirectory", "project");
    std::string programDirectory = Data::getDataValue("programDirectory", "project");
    std::string projectDirectory = Data::getDataValue("projectDirectory", "project");
    std::string recordDirectory = Data::getDataValue("recordDirectory", "project");
    std::string imageDirectory = Data::getDataValue("imageDirectory", "project");

    data.baseDirectory = QString::fromStdString(baseDirectory);
    data.programDirectory = QString::fromStdString(programDirectory);
    data.projectDirectory = QString::fromStdString(projectDirectory);
    data.recordDirectory = QString::fromStdString(recordDirectory);
    data.imageDirectory = QString::fromStdString(imageDirectory);

    return data;
}
void ProjectSettingsController::setDirectories(const DirectoriesData& data) {
    Data::setDataValue("baseDirectory", "project", data.baseDirectory.toStdString());
    Data::setDataValue("programDirectory", "project", data.programDirectory.toStdString());
    Data::setDataValue("projectDirectory", "project", data.projectDirectory.toStdString());
    Data::setDataValue("recordDirectory", "project", data.recordDirectory.toStdString());
    Data::setDataValue("imageDirectory", "project", data.imageDirectory.toStdString());
}

DatabasesData ProjectSettingsController::getDatabases() {

    std::string databaseName = Data::getDataValue("databaseName", "project");
    std::string databaseUrl = Data::getDataValue("databaseUrl", "project");
    std::string toolName = Data::getDataValue("type", "project");
    std::string CMUrl = Data::getDataValue("CMUrl", "project");

    databasesData.databaseName = QString::fromStdString(databaseName);
    databasesData.databaseUrl = QString::fromStdString(databaseUrl);
    databasesData.toolName = QString::fromStdString(toolName);
    databasesData.CMUrl = QString::fromStdString(CMUrl);

    return databasesData;
}

void ProjectSettingsController::setDatabases(const DatabasesData& data) {

    Data::setDataValue("databaseName", "project", data.databaseName.toStdString());
    Data::setDataValue("databaseUrl", "project", data.databaseUrl.toStdString());
    Data::setDataValue("type", "project", data.toolName.toStdString());
    Data::setDataValue("CMUrl", "project", data.CMUrl.toStdString());

    databasesData = data;
}

QStringList ProjectSettingsController::getAdditionalFields() {
    return additionalFields;
}

void ProjectSettingsController::setAdditionalFields(const QStringList& data) {
    additionalFields = data;
}

void ProjectSettingsController::saveProjectBasicInfo() {
    projectInfoRecord::changeProject();
}

QString ProjectSettingsController::generateUniqueId()
{
    QString newId;
    const QString letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int num = QRandomGenerator::global()->bounded(100, 1000); // 3 digits (100–999)
    QString chars;
    for (int i = 0; i < 3; ++i)
        chars += letters[QRandomGenerator::global()->bounded(letters.length())];

    int lastDigit = QRandomGenerator::global()->bounded(0, 10);

    newId = QString("#%1%2%3").arg(num).arg(chars).arg(lastDigit);
    return newId;
}
