#include "usermanagementpage.h"
#include "useraddeditdialog.h"
#include "records.h"
#include <QShowEvent>

UserManagementPage::UserManagementPage(QWidget *parent)
    : QWidget(parent), deleteId(-1), dataInitialized(false)
{
    initializeWidgets();

    connect(createButton, &QPushButton::clicked, this, &UserManagementPage::clickedAddUserButton);
    connect(deleteDialog, &DeleteDialog::requestDelete, this, &UserManagementPage::handleUserDelete);
}

UserManagementPage::~UserManagementPage()
{

}

void UserManagementPage::initializeWidgets() {
    setupUi(this);

    deleteDialog = new DeleteDialog();

    userModel = new QStandardItemModel(this);
    userModel->setColumnCount(5);
    userModel->setHorizontalHeaderLabels({"User ID", "User Name", "Role", "Added on", "Options"});

    usertableView->setModel(userModel);
    usertableView->setShowGrid(false);
    usertableView->setAlternatingRowColors(false);
    usertableView->setSelectionMode(QAbstractItemView::NoSelection);
    usertableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    usertableView->setFocusPolicy(Qt::NoFocus);
    usertableView->setIconSize(QSize(20, 20));
    usertableView->horizontalHeader()->setFixedHeight(46);
    usertableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    usertableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    usertableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    usertableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    usertableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    usertableView->horizontalHeader()->setStretchLastSection(true);
    usertableView->verticalHeader()->setVisible(false);
    usertableView->verticalHeader()->setDefaultSectionSize(60);
    usertableView->resizeColumnsToContents();
}

void UserManagementPage::refresh() {
    if (Data::getDataValue("projectName", "project").empty())
        return;

    userData = ProjectSettingsController::getUsers();

    userModel->removeRows(0, userModel->rowCount());

    int currentRow = 0;
    for (auto id: userData.keys()) {
        QList<QStandardItem*> row;
        row << new QStandardItem(userData[id].ID)
            << new QStandardItem(userData[id].name)
            << new QStandardItem(userData[id].role == 0 ? QString("Admin") : QString("General User"))
            << new QStandardItem(userData[id].addedOn)
            << new QStandardItem();

        userModel->appendRow(row);

        QWidget *actionWidget = new QWidget();
        QPushButton *editBtn = new QPushButton();
        QPushButton *deleteBtn = new QPushButton();

        editBtn->setIcon(QIcon(":/images/Common/TableView/edit.svg"));
        deleteBtn->setIcon(QIcon(":/images/Common/TableView/delete.svg"));
        editBtn->setToolTip("Edit");
        deleteBtn->setToolTip("Delete");

        editBtn->setProperty("userId", id);
        editBtn->setProperty("row", currentRow);
        deleteBtn->setProperty("userId", id);
        deleteBtn->setProperty("row", currentRow);

        connect(editBtn, &QPushButton::clicked, this, &UserManagementPage::handleEditClicked);
        connect(deleteBtn, &QPushButton::clicked, this, &UserManagementPage::handleDeleteClicked);

        QHBoxLayout *layout = new QHBoxLayout(actionWidget);
        layout->addWidget(editBtn);
        layout->addWidget(deleteBtn);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(5);
        layout->addStretch();
        actionWidget->setLayout(layout);

        usertableView->setIndexWidget(userModel->index(currentRow, 4), actionWidget);
        currentRow++;
    }
}

void UserManagementPage::setUsers() {

}

void UserManagementPage::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    
    // Only refresh if a project is loaded (check if projectName exists in Data dictionary)
    std::string projectName = Data::getDataValue("projectName", "project");
    if (projectName.empty())
        return;

    QString project = QString::fromStdString(projectName);
    if (!dataInitialized || loadedProjectName != project) {
        refresh();
        dataInitialized = true;
        loadedProjectName = project;
    }
}

void UserManagementPage::clickedAddUserButton() {
    UserAddEditDialog* dialog = new UserAddEditDialog();
    dialog->show();

    connect(dialog, &UserAddEditDialog::refreshAll, this, &UserManagementPage::refresh);
}


void UserManagementPage::editUser(int pageId, int cellId) {
    UserAddEditDialog* dialog = new UserAddEditDialog(1);
    dialog->setData(cellId);
    dialog->show();
}

void UserManagementPage::deleteUser(int pageId, int cellId) {
    deleteDialog->show();
    deleteId = cellId;
    connect(deleteDialog, &DeleteDialog::requestDelete, this, &UserManagementPage::handleUserDelete);
}

void UserManagementPage::refreshUserByRow(int row) {
    if (row < 0 || row >= userModel->rowCount())
        return;

    int userId = -1;

    // Get userId from the action widget
    QWidget *widget = usertableView->indexWidget(userModel->index(row, 4));
    if (widget) {
        QPushButton *editBtn = widget->findChild<QPushButton*>("", Qt::FindDirectChildrenOnly);
        if (editBtn)
            userId = editBtn->property("userId").toInt();
    }

    if (userId < 0 || !ProjectSettingsController::getUsers().contains(userId))
        return;

    UserData updatedUser = ProjectSettingsController::getUserById(userId);

    userModel->setItem(row, 0, new QStandardItem(updatedUser.ID));
    userModel->setItem(row, 1, new QStandardItem(updatedUser.name));
    userModel->setItem(row, 2, new QStandardItem(updatedUser.role == 0 ? "Admin" : "General User"));
    userModel->setItem(row, 3, new QStandardItem(updatedUser.addedOn));

    // Recreate the edit/delete buttons
    QWidget *actionWidget = new QWidget();
    QPushButton *editBtn = new QPushButton();
    QPushButton *deleteBtn = new QPushButton();

    editBtn->setIcon(QIcon(":/images/Common/TableView/edit.svg"));
    deleteBtn->setIcon(QIcon(":/images/Common/TableView/delete.svg"));
    editBtn->setToolTip("Edit");
    deleteBtn->setToolTip("Delete");

    editBtn->setProperty("userId", userId);
    editBtn->setProperty("row", row);
    deleteBtn->setProperty("userId", userId);
    deleteBtn->setProperty("row", row);

    connect(editBtn, &QPushButton::clicked, this, &UserManagementPage::handleEditClicked);
    connect(deleteBtn, &QPushButton::clicked, this, &UserManagementPage::handleDeleteClicked);

    QHBoxLayout *layout = new QHBoxLayout(actionWidget);
    layout->addWidget(editBtn);
    layout->addWidget(deleteBtn);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);
    layout->addStretch();
    actionWidget->setLayout(layout);

    usertableView->setIndexWidget(userModel->index(row, 4), actionWidget);
}

void UserManagementPage::editUserByRow(int row)
{
    qDebug() << "Edit user id: " << row;
    editUser(row, 0);
}

void UserManagementPage::deleteUserByRow(int row)
{
    deleteUser(row, 0);
}

void UserManagementPage::handleUserDelete() {
    if (deleteId == -1) return;

    ProjectSettingsController::deleteUserById(deleteId);
    refresh();
    deleteId = -1;
}

void UserManagementPage::handleEditClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    int row = btn->property("row").toInt();

    UserAddEditDialog *dialog = new UserAddEditDialog(1);
    dialog->setData(row);
    dialog->show();
    connect(dialog, &UserAddEditDialog::refresh, this, &UserManagementPage::refreshUserByRow);
}

void UserManagementPage::handleDeleteClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;
    deleteId = btn->property("userId").toInt();
    deleteDialog->show();
}
