#ifndef REVIEWITEMCHECKDIALOG_H
#define REVIEWITEMCHECKDIALOG_H

#include <QList>
#include <QVBoxLayout>
#include <QDialog>
#include "ui_reviewitemcheckdialog.h"
#include "reviewitemcheckwidget.h"

class ReviewItemCheckDialog : public QDialog, public Ui::ReviewItemCheckDialog
{
    Q_OBJECT

public:
    explicit ReviewItemCheckDialog(QWidget *parent = nullptr);
    ~ReviewItemCheckDialog();

    void initializeWidgets();
    void setData(int id);

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void clickedCloseButton();
    void clickedCancelButton();
    void clickedSaveButton();

private:
    int index;
    QVBoxLayout* mainLayout;
    QList<ReviewItemCheckWidget*> checkWidgets;
};

#endif // REVIEWITEMCHECKDIALOG_H
