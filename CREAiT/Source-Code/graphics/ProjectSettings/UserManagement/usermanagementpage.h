#ifndef USERMANAGEMENTPAGE_H
#define USERMANAGEMENTPAGE_H

#include <QMap>
#include <QStandardItemModel>
#include <QWidget>
#include "ui_usermanagementpage.h"
#include "deletedialog.h"
#include <projectsettingscontroller.h>

class UserManagementPage : public QWidget, public Ui::UserManagementPage
{
    Q_OBJECT

public:
    explicit UserManagementPage(QWidget *parent = nullptr);
    ~UserManagementPage();

    void initializeWidgets();
    void refresh();
    void setUsers();

protected:
    void showEvent(QShowEvent *event) override;

public slots:
    void clickedAddUserButton();
    void editUser(int pageId, int cellId);
    void deleteUser(int pageId, int cellId);

    void refreshUserByRow(int row);
    void editUserByRow(int row);
    void deleteUserByRow(int row);
    void handleUserDelete();

    void handleEditClicked();
    void handleDeleteClicked();

private:
    QStandardItemModel *userModel;
    DeleteDialog* deleteDialog;
    QMap<int, UserData> userData;
    int deleteId;
    bool dataInitialized;
    QString loadedProjectName;
};

#endif // USERMANAGEMENTPAGE_H
