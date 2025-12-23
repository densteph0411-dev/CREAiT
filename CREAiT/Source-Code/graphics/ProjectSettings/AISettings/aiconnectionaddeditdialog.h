#ifndef AICONNECTIONADDEDITDIALOG_H
#define AICONNECTIONADDEDITDIALOG_H

#include <QDialog>
#include "ui_aiconnectionaddeditdialog.h"
#include <projectsettingscontroller.h>


class AIConnectionAddEditDialog : public QDialog, public Ui::AIConnectionAddEditDialog
{
    Q_OBJECT

public:
    explicit AIConnectionAddEditDialog(int type = 0, QWidget *parent = nullptr);
    ~AIConnectionAddEditDialog();

    void setData(int id);

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

signals:
    void requestRefreshAll();
    void requestRefresh(int pageId, int cellId);

public slots:
    void clickedCloseButton();
    void clickedCancelButton();
    void clickedSaveButton();

private:
    int type;
    int aiConnectionId;
};

#endif // AICONNECTIONADDEDITDIALOG_H
