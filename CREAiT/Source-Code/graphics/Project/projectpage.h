#ifndef PROJECTPAGE_H
#define PROJECTPAGE_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QStandardItemModel>
#include <Presenter/projectdatapresenter.h>
#include "ProjectTreeDelegate.h"
#include "projecttreemodel.h"

namespace Ui {
class ProjectPage;
}

class ProjectPage : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectPage(QWidget *parent = nullptr);
    ~ProjectPage();

    const static int RECORD_TREEITEM = 0;
    const static int DIR_TREEITEM = 1;

    ProjectTreeModel model;
    ProjectTreeDelegate *treeDelegate;
public slots:
    void onTreeItemClicked(const QModelIndex &index);
    void onTreeItemDoubleClicked(const QModelIndex &index);
    void onTreeContextMenuRequested(const QPoint &pos);

    void onRecordsButtonToggled(bool value);
    void onRecordsButtonClicked(bool value);

    void onTreeItemMoreActionClicked(const QModelIndex &index);
    void onTreeItemPlusActionClicked(const QModelIndex &index);

    void onNewDirectoryMenu();
    void onNewRecordMenu();
    void onAddRecordBelowMenu();
    void onDeleteRecrodMenu();
    void onRenameDirectoryMenu();
    void onDeleteDirectoryMenu();
    void onCreateDirectoryMenu();
    void onCreateRecordMenu();

    void onRecordCreated(QString recID);
    void onRecordUpdated(QString recID);
    void onRecordDeleted(QString recID);
    void onCancelEditRecord(QString recID);

    void onSwitchEditRecordForm(QString recID);

protected:
    void showEvent(QShowEvent *event) override;

    void showCreateRecordForm(QString parent = "PROJ", QString treeViewIndex = "");
    void loadRecordsForProject();

    QStandardItem* createTreeItem(Record record);
    QIcon createColoredSvgIcon(const QString &svgPath, const QString &color, const QSize &size);
protected slots:
    void onRecordMoved(QStandardItem* srcItem, QStandardItem* parentItem, int rowIndex);

private slots:
    void on_ProjectInfoButton_clicked();

private:
    Ui::ProjectPage *ui;

    QMap<QString, QString> recordTypeIcons;

    QMenu *newDirRecordMenu;
    QMenu *directoryMoreActionMenu;
    QMenu *directoryPlusActionMenu;
    QMenu *recordMoreActionMenu;

    QModelIndex selectedTreeItemIndex;
    QString selectedRecordID;
};

#endif // PROJECTPAGE_H
