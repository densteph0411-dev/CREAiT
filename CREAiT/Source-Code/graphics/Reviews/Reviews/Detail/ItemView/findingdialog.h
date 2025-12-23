#ifndef FINDINGDIALOG_H
#define FINDINGDIALOG_H

#include <QDialog>
#include "ui_findingdialog.h"

class FindingDialog : public QDialog, public Ui::FindingDialog
{
    Q_OBJECT

public:
    explicit FindingDialog(QWidget *parent = nullptr);
    ~FindingDialog();

    void initializeWidgets();
    void setData(const QString& reviewItemName);

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    int index;
};

#endif // FINDINGDIALOG_H
