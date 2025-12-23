#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>
#include "ui_loadingdialog.h"
#include "spinnerwidget.h"

class LoadingDialog : public QDialog, public Ui::LoadingDialog
{
    Q_OBJECT

public:
    explicit LoadingDialog(QWidget *parent = nullptr);
    ~LoadingDialog();

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    SpinnerWidget* spinner;
};

#endif // LOADINGDIALOG_H
