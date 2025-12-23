#ifndef USERADDEDITDIALOG_H
#define USERADDEDITDIALOG_H

#include <QDialog>
#include "ui_useraddeditdialog.h"
#include <projectsettingscontroller.h>

class UserAddEditDialog : public QDialog, public Ui::UserAddEditDialog
{
    Q_OBJECT

public:
    explicit UserAddEditDialog(int type = 0, QWidget *parent = nullptr);
    ~UserAddEditDialog();

    void setData(int id);

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

signals:
    void refreshAll();
    void refresh(int userId);

public slots:
    void clickedCloseButton();
    void clickedCancelButton();
    void clickedSaveButton();

private:
    int type;
    int userId;
};

#endif // USERADDEDITDIALOG_H
