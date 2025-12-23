#ifndef RECORDADDLINKDIALOG_H
#define RECORDADDLINKDIALOG_H
#include <QDialog>
#include <QTimer>
#include <RecordSummaryPopover.h>
#include "linkrecordtablemodel.h"

namespace Ui {
class RecordAddLinkDialog;
}

class RecordAddLinkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecordAddLinkDialog(QString recID, QWidget *parent = nullptr);
    ~RecordAddLinkDialog();
public slots:
    void onCloseButtonClicked();
    void onNextButtonClicked();
    void onSearchChanged(QString text);
protected:
    void showEvent(QShowEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;
private:
    QString m_recordID;
    LinkRecordTableModel* model;
    Ui::RecordAddLinkDialog *ui;
    int stepIndex;
    QTimer searchTimer;
    QTimer previewTimer;

    QString pendingRecID;
    QModelIndex pendingIndex;
};

#endif // RECORDADDLINKDIALOG_H
