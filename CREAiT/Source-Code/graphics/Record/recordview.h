#ifndef RECORDVIEW_H
#define RECORDVIEW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTimer>

namespace Ui {
class RecordView;
}

class RecordView : public QWidget
{
    Q_OBJECT

public:
    explicit RecordView(QWidget *parent = nullptr);
    ~RecordView();

    void loadRecord(QString recordID);
public slots:
    void onVersionHistoryButtonClicked();
    void onRecordLinkButtonClicked();
    void onEditRecordButtonClicked();
    void onAddLinkButtonClicked();
    void onMouseHoverLinkTableItem(QModelIndex index);

signals:
    void switchToEditRecordView(QString recID);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::RecordView *ui;

    QStandardItemModel *additionalFieldModel;
    QStandardItemModel *linksModel;

    QString currentRecordID;

    QTimer previewTimer;

    QString pendingRecID;
    QModelIndex pendingIndex;
};

#endif // RECORDVIEW_H
