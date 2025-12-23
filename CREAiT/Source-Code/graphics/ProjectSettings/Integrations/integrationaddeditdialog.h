#ifndef INTEGRATIONADDEDITDIALOG_H
#define INTEGRATIONADDEDITDIALOG_H

#include <QDialog>
#include "ui_integrationaddeditdialog.h"

class IntegrationAddEditDialog : public QDialog, public Ui::IntegrationAddEditDialog
{
    Q_OBJECT

public:
    explicit IntegrationAddEditDialog(int type = 0, QWidget *parent = nullptr);
    ~IntegrationAddEditDialog();

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
    int integrationId;
};

#endif // INTEGRATIONADDEDITDIALOG_H
