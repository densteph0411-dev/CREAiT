#ifndef PROJECTSETTINGSCONTROLLER_H
#define PROJECTSETTINGSCONTROLLER_H

#include <QMap>
#include <QRandomGenerator>
#include <QString>
#include <QStringList>
#include <records.h>

struct BasicInfoData {
    QString projectName;
    QString type;
    QString IDPrefix;
    QString credentialsFileName;
    QString description;
};

struct DirectoriesData {
    QString baseDirectory;
    QString programDirectory;
    QString projectDirectory;
    QString recordDirectory;
    QString imageDirectory;
};

struct DatabasesData {
    QString databaseName;
    QString databaseUrl;
    QString CMUrl;
    QString toolName;
};

struct IntegrationData {
    int toolDisplayIndex;
    QString id;
    QString version;
    QString title;
    QString connectionDate;
    QString url;
    QString Key;
    QString username;
    QString password;
};

struct AIConnectionData {
    QString ID;
    QString url;
    QString aiModel;
    QString version;
    QString Key;
};

struct UserData {
    QString ID;
    QString name;
    QString password;
    int role;
    QString addedOn;
};


class ProjectSettingsController
{
public:
    ProjectSettingsController();

    static BasicInfoData getBasicInfo();
    static void setBasicInfo(const BasicInfoData& data);

    static DirectoriesData getDirectories();
    static void setDirectories(const DirectoriesData& data);

    static DatabasesData getDatabases();
    static void setDatabases(const DatabasesData& data);

    static QStringList getAdditionalFields();
    static void setAdditionalFields(const QStringList& data);

    static void saveProjectBasicInfo();

    // IntegrationData CRUD
    static void addIntegration(const IntegrationData& data);
    static void updateIntegration(int id, const IntegrationData& data);
    static void deleteIntegrationById(int id);
    static IntegrationData getIntegrationById(int id);
    static QMap<int, IntegrationData> getAllIntegrations();

    // AIConnectionData CRUD
    static void addAIConnection(const AIConnectionData& data);
    static void updateAIConnection(int id, const AIConnectionData& data);
    static void deleteAIConnectionById(int id);
    static AIConnectionData getAIConnectionById(int id);
    static QMap<int, AIConnectionData> getAllAIConnections();

    // UserData CRUD
    static void addUser(const UserData& data);
    static void updateUser(int id, const UserData& data);
    static void deleteUserById(int id);
    static UserData getUserById(int id);
    static QMap<int, UserData> getUsers();

    static QString generateUniqueId();

private:
    static DatabasesData databasesData;
    static QStringList additionalFields;

    static QMap<int, IntegrationData> integrations;
    static QMap<int, AIConnectionData> aiConnections;
    static QMap<int, UserData> users;

    static int integrationIdCounter;
    static int aiConnectionIdCounter;
    static int userIdCounter;
};

#endif // PROJECTSETTINGSCONTROLLER_H
