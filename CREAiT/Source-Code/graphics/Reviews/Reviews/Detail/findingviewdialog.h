#ifndef FINDINGVIEWDIALOG_H
#define FINDINGVIEWDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include "ui_findingviewdialog.h"
#include <reviewscontroller.h>

class FindingViewDialog : public QDialog, public Ui::FindingViewDialog
{
    Q_OBJECT

public:
    explicit FindingViewDialog(QWidget *parent = nullptr);
    ~FindingViewDialog();

    void initializeWidgets();
    void setData(const QString& reviewId, const QString& reviewItemId, const QList<FindingData>& data);

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QVBoxLayout* mainLayout;
};

#endif // FINDINGVIEWDIALOG_H
