#include "appcontextcontroller.h"

AppContextController::AppContextController() {}

// Initialize the static member
UserInfo AppContextController::userInfo = {
    "Test Admin",
    "Asd2did5",
    "test@mail.com",
    {
        "Add Records",
        "Add Directory",
        "Edit Finding",
        "Verify Findings",
        "Create Directory",
        "Manage Users",
        "AI Connections",
        "Reject Findings",
        "Create Reviews",
        "Edit Record",
        "Integration",
        "Create Findings"
    }
};


UserInfo AppContextController::getUserInfo() {

    // Load the values from records.
    userInfo.username = QString::fromStdString(Data::getDataValue("userID","credentials"));
    userInfo.password = QString::fromStdString(Data::getDataValue("passWord","credentials"));
    userInfo.email = QString::fromStdString(Data::getDataValue("email","credentials"));

    return userInfo;
}

void AppContextController::updateUserInfo(const UserInfo& info) {
    userInfo.username = info.username;
    userInfo.password = info.password;
    userInfo.email = info.email;
    userInfo.permissions = info.permissions;

    Data::setDataValue("userID","credentials",userInfo.username.toStdString());
    Data::setDataValue("passWord","credentials",userInfo.password.toStdString());
    Data::setDataValue("email","credentials",userInfo.email.toStdString());
}
