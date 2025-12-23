#ifndef AUTOMATIONTESTRESULTDIALOG_H
#define AUTOMATIONTESTRESULTDIALOG_H

#include <QDialog>
#include "ui_automationtestresultdialog.h"


class AutomationTestResultDialog : public QDialog, public Ui::AutomationTestResultDialog
{
    Q_OBJECT

public:
    explicit AutomationTestResultDialog(QWidget *parent = nullptr);
    ~AutomationTestResultDialog();

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);
};

#endif // AUTOMATIONTESTRESULTDIALOG_H
