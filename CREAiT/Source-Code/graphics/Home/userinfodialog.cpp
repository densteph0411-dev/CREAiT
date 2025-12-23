#include "userinfodialog.h"
#include <QPainter>
#include <QGridLayout>
#include "appcontextcontroller.h"

UserInfoDialog::UserInfoDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    setupUi(this);

    permissionContainerLayout = new QGridLayout(permissionContainer);
    permissionContainerLayout->setContentsMargins(0, 0, 0, 0);
    permissionContainerLayout->setSpacing(10);

    connect(closeButton, &QPushButton::clicked, this, &UserInfoDialog::clickedCloseButton);
    connect(updateButton, &QPushButton::clicked, this, &UserInfoDialog::clickedUpdateButton);
}

UserInfoDialog::~UserInfoDialog() {

}

void UserInfoDialog::setData(int userId) {
    UserInfo userInfo = AppContextController::getUserInfo();

    QLayoutItem *item;
    while ((item = permissionContainerLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    const int columns = 3;
    int row = 0, col = 0;

    usernameEdit->setText(userInfo.username);
    passwordEdit->setText(userInfo.password);
    emailEdit->setText(userInfo.email);

    for (auto str: userInfo.permissions) {
        QLabel* permissionLabel = new QLabel(str);
        permissionLabel->setStyleSheet({R"(
            font-family: Open Sans;
            font-weight: 500;
            font-size: 16px;
            line-height: 150%;
            letter-spacing: 0%;
            color: #4D4D4D;
        )"});
        permissionContainerLayout->addWidget(permissionLabel, row, col);
        col++;
        if (col >= columns) {
            col = 0;
            row++;
        }
    }
}

void UserInfoDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void UserInfoDialog::showEvent(QShowEvent *event) {
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}

void UserInfoDialog::clickedCloseButton() {
    reject();
}

void UserInfoDialog::clickedUpdateButton() {
    UserInfo info;

    UserInfo userInfo = AppContextController::getUserInfo(); // for test

    info.username = usernameEdit->text();
    info.password = passwordEdit->text();
    info.email = emailEdit->text();
    info.permissions = userInfo.permissions;

    AppContextController::updateUserInfo(info);

    accept();
}
