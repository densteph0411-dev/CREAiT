#ifndef REVIEWITEMCHECKRESULTDIALOG_H
#define REVIEWITEMCHECKRESULTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include "ui_reviewitemcheckresultdialog.h"

class ReviewItemCheckResultDialog : public QDialog, public Ui::ReviewItemCheckResultDialog
{
    Q_OBJECT

public:
    explicit ReviewItemCheckResultDialog(QWidget *parent = nullptr);
    ~ReviewItemCheckResultDialog();

    void initializeWidgets();
    void setData(int id);

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QVBoxLayout* mainLayout;
};

#endif // REVIEWITEMCHECKRESULTDIALOG_H
