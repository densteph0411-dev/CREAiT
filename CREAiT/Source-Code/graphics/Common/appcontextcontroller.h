#ifndef APPCONTEXTCONTROLLER_H
#define APPCONTEXTCONTROLLER_H

#include <records.h>
#include <QStringList>

struct UserInfo {
    QString username;
    QString password;
    QString email;
    QStringList permissions;
};

class AppContextController
{
public:
    AppContextController();

    static UserInfo getUserInfo();
    static void updateUserInfo(const UserInfo& userInfo);

private:
    static UserInfo userInfo;
};

#endif // APPCONTEXTCONTROLLER_H
