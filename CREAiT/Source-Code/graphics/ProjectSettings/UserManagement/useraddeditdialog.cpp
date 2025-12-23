#include "useraddeditdialog.h"
#include <QDate>
#include <QPainter>

UserAddEditDialog::UserAddEditDialog(int type, QWidget *parent)
    : QDialog(parent), type(type)
{
    setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    roleSelect->addItems({ "Admin", "General User" });

    connect(closeButton, &QPushButton::clicked, this, &UserAddEditDialog::clickedCloseButton);
    connect(cancelButton, &QPushButton::clicked, this, &UserAddEditDialog::clickedCancelButton);
    connect(saveButton, &QPushButton::clicked, this, &UserAddEditDialog::clickedSaveButton);
}

UserAddEditDialog::~UserAddEditDialog()
{

}

void UserAddEditDialog::setData(int id) {
    userId = id;

    UserData data = ProjectSettingsController::getUserById(id);

    usernameEdit->setText(data.name);
    passwordEdit->setText(data.password);
    roleSelect->setCurrentIndex(data.role);
}

void UserAddEditDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void UserAddEditDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}

void UserAddEditDialog::clickedCancelButton() {
    emit clickedCloseButton();
}

void UserAddEditDialog::clickedCloseButton() {
    reject();
}

void UserAddEditDialog::clickedSaveButton() {
    QString dateStr = QDate::currentDate().toString("MMMM d, yyyy");

    UserData data = ProjectSettingsController::getUserById(userId);
    data.name = usernameEdit->text();
    data.password = passwordEdit->text();
    data.role = roleSelect->currentIndex();
    data.addedOn = dateStr;

    if (type == 0) {
        data.ID = ProjectSettingsController::generateUniqueId();
        ProjectSettingsController::addUser(data);
        emit refreshAll();
    } else {
        ProjectSettingsController::updateUser(userId, data);
        emit refresh(userId);
    }
    accept();
}
