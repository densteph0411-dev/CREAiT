#ifndef USERINFODIALOG_H
#define USERINFODIALOG_H

#include <QDialog>
#include <QStringList>
#include "ui_userinfodialog.h"

class UserInfoDialog : public QDialog, public Ui::UserInfoDialog
{
    Q_OBJECT

public:
    explicit UserInfoDialog(QWidget *parent = nullptr);
    ~UserInfoDialog();
    
    void setData(int userId = 0);
protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

public slots:
    void clickedCloseButton();
    void clickedUpdateButton();

private:
    QStringList permissions;
    QGridLayout* permissionContainerLayout;
};

#endif // USERINFODIALOG_H
