#ifndef REVIEWACTIONDIALOG_H
#define REVIEWACTIONDIALOG_H

#include <QDialog>
#include "ui_reviewactiondialog.h"

class ReviewActionDialog : public QDialog, public Ui::ReviewActionDialog
{
    Q_OBJECT

public:
    explicit ReviewActionDialog(QWidget *parent = nullptr);
    ~ReviewActionDialog();

    void initializeWidgets();
    void setData(int id, int type, int versionId, const QString& version, const QString& description);

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);


private slots:
    void clickedCloseButton();
    void clickedCancelButton();
    void clickedSaveButton();

public:
    enum {
        Implement,
        Comment,
        VerifyFinding,
        RejectFinding
    };

private:
    int index;
    int actionType;
};

#endif // REVIEWACTIONDIALOG_H
