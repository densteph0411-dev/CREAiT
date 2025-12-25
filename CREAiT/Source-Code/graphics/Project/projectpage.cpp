#include "projectpage.h"
#include "qdir.h"
#include "ui_projectpage.h"

#include <QElapsedTimer>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QStandardItemModel>
#include <QMenu>
#include <QTimer>
#include <queue>
#include <QSvgRenderer>
#include "Dialogs/createdirectorydialog.h"
#include "Dialogs/deletedialog.h"
#include "Record/recordview.h"
#include "Presenter/recorddatapresenter.h"
#include <Presenter/projectdatapresenter.h>

#include <QPainter>
#include <QFile>

#define TREEITEM_RECORD_ID_ROLE  Qt::UserRole
#define TREEITEM_PARENT_ID_ROLE  Qt::UserRole + 1

ProjectPage::ProjectPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProjectPage)
{
    ui->setupUi(this);

    QAction *searchIcon = new QAction(QIcon(":/images/search.svg"), "", ui->searchLineEdit);
    ui->searchLineEdit->addAction(searchIcon, QLineEdit::LeadingPosition);

    ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();

    connect(&projInstance, &ProjectDataPresenter::recordCreated, this, &ProjectPage::onRecordCreated);
    connect(&projInstance, &ProjectDataPresenter::recordDeleted, this, &ProjectPage::onRecordDeleted);
    // ====================== Initialize Record Tree =====================

    connect(&model, &QStandardItemModel::dataChanged, this, [=, this](const QModelIndex &topLeft, const QModelIndex &, const QVector<int> &) {
        QString newName = model.data(topLeft).toString();
        QString recID = model.data(topLeft, Qt::UserRole).toString();

        ProjectDataPresenter &instance = ProjectDataPresenter::instance();
        instance.renameRecord(recID, newName);
    });

    connect(ui->treeView, &ProjectTreeView::recordMoved, this, &ProjectPage::onRecordMoved);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested,
            this, &ProjectPage::onTreeContextMenuRequested);
    // ======================================================================

    // ==================== Create Context Menu for New Directory and Record ===============
    newDirRecordMenu = new QMenu();
    newDirRecordMenu->addAction("New Directory", this, SLOT(onNewDirectoryMenu()));
    newDirRecordMenu->addAction("New Record", this, SLOT(onNewRecordMenu()));

    newDirRecordMenu->setStyleSheet(R"(
        QMenu {
            font-family: Open Sans;
            font-weight: 500;
            font-size: 14px;
            line-height: 22px;
            letter-spacing: 0%;
            color: #1A1A1A;
            background-color: white;
            border: 1px solid #ccc;
            border-radius: 8px;
            padding: 12px;
        }
        QMenu::item {
            padding: 6px 12px;
            border-radius: 6px;
        }
        QMenu::item:selected {
            background-color: #f0f0f0;
        }
    )");

    connect(ui->plusRecordButton, &QPushButton::clicked, this, [=, this]() {
        QPoint globalPos = ui->plusRecordButton->mapToGlobal(QPoint(0, ui->plusRecordButton->height()));
        newDirRecordMenu->exec(globalPos);
    });
    // ======================================================================


    // ==================== Create Context Menu for New Directory and Record ===============
    recordMoreActionMenu = new QMenu();
    recordMoreActionMenu->addAction("Add Record Below", this, SLOT(onAddRecordBelowMenu()));
    recordMoreActionMenu->addAction("Delete", this, SLOT(onDeleteRecrodMenu()));

    recordMoreActionMenu->setStyleSheet(R"(
        QMenu {
            font-family: Open Sans;
            font-weight: 500;
            font-size: 14px;
            line-height: 22px;
            letter-spacing: 0%;
            color: #1A1A1A;
            background-color: white;
            border: 1px solid #ccc;
            border-radius: 8px;
            padding: 12px;
        }
        QMenu::item {
            padding: 6px 12px;
            border-radius: 6px;
        }
        QMenu::item:selected {
            background-color: #f0f0f0;
        }
    )");

    directoryMoreActionMenu = new QMenu();
    directoryMoreActionMenu->addAction("Rename", this, SLOT(onRenameDirectoryMenu()));
    directoryMoreActionMenu->addAction("Delete", this, SLOT(onDeleteDirectoryMenu()));

    directoryMoreActionMenu->setStyleSheet(R"(
        QMenu {
            font-family: Open Sans;
            font-weight: 500;
            font-size: 14px;
            line-height: 22px;
            letter-spacing: 0%;
            color: #1A1A1A;
            background-color: white;
            border: 1px solid #ccc;
            border-radius: 8px;
            padding: 12px;
        }
        QMenu::item {
            padding: 6px 12px;
            border-radius: 6px;
        }
        QMenu::item:selected {
            background-color: #f0f0f0;
        }
    )");

    directoryPlusActionMenu = new QMenu();
    directoryPlusActionMenu->addAction("Create Directory", this, SLOT(onCreateDirectoryMenu()));
    directoryPlusActionMenu->addAction("Create Record", this, SLOT(onCreateRecordMenu()));

    directoryPlusActionMenu->setStyleSheet(R"(
        QMenu {
            font-family: Open Sans;
            font-weight: 500;
            font-size: 14px;
            line-height: 22px;
            letter-spacing: 0%;
            color: #1A1A1A;
            background-color: white;
            border: 1px solid #ccc;
            border-radius: 8px;
            padding: 12px;
        }
        QMenu::item {
            padding: 6px 12px;
            border-radius: 6px;
        }
        QMenu::item:selected {
            background-color: #f0f0f0;
        }
    )");
    
    // ======================================================================

    connect(ui->editRecordForm, SIGNAL(recordUpdated(QString)), this, SLOT(onRecordUpdated(QString)));
    connect(ui->editRecordForm, SIGNAL(cancelEditRecord(QString)), this, SLOT(onCancelEditRecord(QString)));
    connect(ui->recordView, SIGNAL(switchToEditRecordView(QString)), this, SLOT(onSwitchEditRecordForm(QString)));
}

ProjectPage::~ProjectPage()
{
    delete ui;
}

void ProjectPage::loadRecordsForProject()
{
    ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();

    std::map<std::string, QStandardItem*> recordItemMap;
    QList<Record> recordList = projInstance.fetchAllRecordList();

    model.clear();

    // Step 1: Load all records into a map
    for (const auto &record : recordList) {
        QStandardItem *item = createTreeItem(record);
        recordItemMap[record.id.toStdString()] = item;
    }

    // Step 2: Build tree hierarchy after all items are created

    std::string rootID = "PROJ";
    std::queue<std::string> q;
    q.push(rootID);

    while (!q.empty()) {
        std::string currentID = q.front();

        if (currentID != "PROJ") {
            QStandardItem *curItem = recordItemMap[currentID];
            std::string parentID = curItem->data(Qt::UserRole + 1).toString().toStdString();
            bool deleted = curItem->data(Qt::UserRole + 4).toBool();
            if (!deleted) {
                QStandardItem *parentItem = nullptr;
                if (!parentID.empty() && recordItemMap.count(parentID)) {
                    parentItem = recordItemMap[parentID];
                }

                if (!parentItem || parentID == "PROJ") parentItem = model.invisibleRootItem();


                parentItem->appendRow(curItem);
            }
        }

        q.pop();

        QList<Record> childList = projInstance.fetchChildRecordList(QString::fromStdString(currentID));
        // Push children to queue
        for (int i=0; i<childList.count(); i++) {
            q.push(childList[i].id.toStdString());
        }
    }

    ui->treeView->setModel(&model);
    ui->treeView->expandAll();  // optional
    ui->treeView->setIndentation(10);
    ui->treeView->setIconSize(QSize(16, 16));

    treeDelegate = new ProjectTreeDelegate(ui->treeView);
    ui->treeView->setItemDelegate(treeDelegate);

    connect(ui->treeView, &ProjectTreeView::actionMoreButtonClicked,
            this, &ProjectPage::onTreeItemMoreActionClicked);
    connect(ui->treeView, &ProjectTreeView::actionPlusButtonClicked,
            this, &ProjectPage::onTreeItemPlusActionClicked);

    ui->projectInfoView->loadProjectInfoData();
}

// QIcon ProjectPage::createColoredSvgIcon(const QString &svgPath, const QString &color, const QSize &size)
// {
//     QFile file(svgPath);
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//         return QIcon();

//     QString contents = file.readAll();
//     file.close();

//     // Replace the fill color (adjust to match your SVG's color usage)
//     contents.replace("#000000", color, Qt::CaseInsensitive);

//     // Load into QSvgRenderer from QByteArray
//     QByteArray svgData = contents.toUtf8();
//     QSvgRenderer renderer(svgData);

//     QPixmap pixmap(size);
//     pixmap.fill(Qt::transparent);
//     QPainter painter(&pixmap);
//     renderer.render(&painter);

//     return QIcon(pixmap);
// }

QIcon ProjectPage::createColoredSvgIcon(const QString &svgPath,
                                        const QString &color,
                                        const QSize &size)
{
    QFile file(svgPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QIcon();

    QString contents = QString::fromUtf8(file.readAll());
    file.close();

    contents.replace("#000000", color, Qt::CaseInsensitive);

    QSvgRenderer renderer(contents.toUtf8());
    if (!renderer.isValid())
        return QIcon();

    qreal dpr = 1.0;
    if (QGuiApplication::primaryScreen())
        dpr = QGuiApplication::primaryScreen()->devicePixelRatio();
    if (dpr < 1.0) dpr = 1.0;

    auto renderOne = [&](QSize logical)->QPixmap {
        QSize px(qMax(1, int(logical.width() * dpr)),
                 qMax(1, int(logical.height() * dpr)));
        QPixmap pm(px);
        pm.fill(Qt::transparent);
        pm.setDevicePixelRatio(dpr);

        QPainter p(&pm);
        p.setRenderHint(QPainter::Antialiasing, true);
        p.setRenderHint(QPainter::SmoothPixmapTransform, true);
        renderer.render(&p, QRect(QPoint(0,0), logical)); // logical rect
        return pm;
    };QIcon icon;
    icon.addPixmap(renderOne(size));
    icon.addPixmap(renderOne(QSize(20,20)));
    icon.addPixmap(renderOne(QSize(24,24)));
    icon.addPixmap(renderOne(QSize(32,32)));
    icon.addPixmap(renderOne(QSize(48,48)));
    return icon;
}

QStandardItem* ProjectPage::createTreeItem(Record record)
{
    QStandardItem *item = new QStandardItem(record.title);

    item->setSizeHint(QSize(0, 28));
    item->setFlags(item->flags() | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);

    item->setData(record.id, Qt::UserRole);
    item->setData(record.parent, Qt::UserRole + 1);
    item->setData(record.type, Qt::UserRole + 2);
    item->setData(record.treeViewIndex, Qt::UserRole + 3);
    item->setData(record.deleted, Qt::UserRole + 4);

    QString iconPath = ProjectDataPresenter::getRecordIconPath(record);
    QString iconColor = "#1A1A1A";
    if (record.type == "Directory") {
        iconColor = "#1A1A1A";
    } else {
        if (record.status == "Draft") {
            iconColor = "#B82132";
        } else if (record.status == "Review") {
            iconColor = "#FEBA17";
        } else if (record.status == "Reviewed") {
            iconColor = "#3A7D44";
        }
    }

    QIcon icon = createColoredSvgIcon(iconPath, iconColor, QSize(16,16));

    item->setIcon(QIcon(icon));

    return item;
}

void ProjectPage::on_ProjectInfoButton_clicked()
{
    ui->ProjectInfoButton->setChecked(true);
    ui->treeView->clearSelection();

    ui->ViewPart->setCurrentWidget(ui->projectInfoView);
}

void ProjectPage::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event); // Call base implementation

    // Call loadRecordsForProject only once or whenever shown
    loadRecordsForProject();
}

void ProjectPage::onRecordMoved(QStandardItem* srcItem, QStandardItem* parentItem, int rowIndex)
{
    QString sourceID = srcItem->data(Qt::UserRole).toString();
    QString targetID = parentItem->data(Qt::UserRole).toString();
    QString treeViewIndex = "";

    ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();

    QModelIndexList matches = model.match(
        model.index(0, 0),              // start from root
        Qt::UserRole + 3,               // treeViewIndex is stored here
        QVariant::fromValue(sourceID),     // value to match
        1,                              // only one match
        Qt::MatchExactly | Qt::MatchRecursive
    );

    for (const QModelIndex &index : matches) {
        QStandardItem *item = model.itemFromIndex(index);
        if (item != nullptr) { // check for null pointer
            // Do something with the item...
            qDebug() << "Found matching item: " << item->text();
            QString oldBelowRecordID = item->data(Qt::UserRole).toString();
            QString parentID = item->data(Qt::UserRole + 1).toString();
            QString oldTreeViewIndex = srcItem->data(Qt::UserRole + 3).toString();

            projInstance.moveRecord(oldBelowRecordID, parentID, oldTreeViewIndex);
        }
    }

    QModelIndex targetParentIndex = model.indexFromItem(parentItem);

    if (rowIndex - 1 >= 0) {
        QModelIndex aboveItemIndex = model.index(rowIndex - 1, 0, targetParentIndex);
        if (aboveItemIndex.isValid()) {
            treeViewIndex = aboveItemIndex.data(Qt::UserRole).toString();
        }
    } else {
        treeViewIndex = "";
    }

    projInstance.moveRecord(sourceID, targetID, treeViewIndex);

    QModelIndex belowItemIndex = model.index(rowIndex + 1, 0, targetParentIndex);
    if (belowItemIndex.isValid()) {
        QString belowID = belowItemIndex.data(Qt::UserRole).toString();
        projInstance.moveRecord(belowID, targetID, sourceID);
    }
}

void ProjectPage::onRecordsButtonToggled(bool value)
{

}
void ProjectPage::onRecordsButtonClicked(bool value)
{
    ui->ProjectInfoButton->setChecked(false);
}

void ProjectPage::onNewDirectoryMenu()
{
    CreateDirectoryDialog dlg(this);
    int result = dlg.exec();

    ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();

    if (result == QDialog::Accepted) {
        QString title = dlg.directoryName();
        QString parentId = dlg.parentID();
        if (parentId.isEmpty()) {
            parentId = "PROJ";
        }

        QList<Record> childList = projInstance.fetchChildRecordList(parentId);
        Record lastChild = childList.last();

        QString recID = projInstance.createRecord("Directory", title, parentId, lastChild.id, "");
    }
}

void ProjectPage::onNewRecordMenu()
{
    ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();

    QList<Record> childList = projInstance.fetchChildRecordList("PROJ");
    Record lastChild = childList.last();

    showCreateRecordForm("PROJ", lastChild.id);
}

void ProjectPage::onAddRecordBelowMenu()
{
    QString parentId = selectedTreeItemIndex.data(Qt::UserRole+1).toString();
    showCreateRecordForm(parentId, selectedRecordID);
}

void ProjectPage::onDeleteRecrodMenu()
{
    DeleteDialog dlg(tr(""), this);
    int result = dlg.exec();

    if (result == QDialog::Accepted) {
        ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();
        projInstance.deleteRecord(selectedRecordID);
    }
}

void ProjectPage::onRenameDirectoryMenu()
{
    if (!selectedTreeItemIndex.isValid())
        return;

    ui->treeView->edit(selectedTreeItemIndex);
}

void ProjectPage::onDeleteDirectoryMenu()
{
    DeleteDialog dlg(tr(""), this);
    int result = dlg.exec();

    if (result == QDialog::Accepted) {
        ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();
        projInstance.deleteRecord(selectedRecordID);
    }
}

void ProjectPage::onCreateDirectoryMenu()
{
    CreateDirectoryDialog dlg(this);
    dlg.setParentID(selectedRecordID);
    int result = dlg.exec();

    if (result == QDialog::Accepted) {
        QString title = dlg.directoryName();
        QString parentId = dlg.parentID();
        if (parentId.isEmpty()) {
            parentId = "PROJ";
        }

        ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();

        QList<Record> childList = projInstance.fetchChildRecordList(parentId);
        Record lastChild = childList.last();

        QString dirID = projInstance.createRecord("Directory",title, parentId, lastChild.id, "");
    }
}

void ProjectPage::onCreateRecordMenu()
{
    ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();

    QList<Record> childList = projInstance.fetchChildRecordList(selectedRecordID);
    Record lastChild = childList.last();

    showCreateRecordForm(selectedRecordID, lastChild.id);
}

void ProjectPage::onTreeItemClicked(const QModelIndex &index)
{
    QString recordID = index.data(TREEITEM_RECORD_ID_ROLE).toString();

    ui->recordView->loadRecord(recordID);
    ui->ViewPart->setCurrentWidget(ui->recordView);
}

void ProjectPage::onTreeItemDoubleClicked(const QModelIndex &index)
{
    QString recordID = index.data(TREEITEM_RECORD_ID_ROLE).toString();

    RecordView *recordView = new RecordView();
    recordView->loadRecord(recordID);
    recordView->show();
}

void ProjectPage::onTreeContextMenuRequested(const QPoint &pos) {
    QModelIndex index = ui->treeView->indexAt(pos);
    if (!index.isValid())
        return;

    QPoint globalPos = ui->treeView->viewport()->mapToGlobal(pos);
    // Optionally: check if it's a valid "Record" item
    if (index.data(Qt::UserRole + 2).toString() == "Directory"){
        selectedRecordID = index.data(Qt::UserRole).toString();
        selectedTreeItemIndex = index;
        directoryMoreActionMenu->exec(globalPos);
    } else {
        selectedRecordID = index.data(Qt::UserRole).toString();
        selectedTreeItemIndex = index;
        recordMoreActionMenu->exec(globalPos);
    }
}

void ProjectPage::onTreeItemMoreActionClicked(const QModelIndex &index)
{
    QPoint globalPos = ui->treeView->mapToGlobal(QPoint(ui->treeView->visualRect(index).right()-80, ui->treeView->visualRect(index).bottom()));
    if (index.data(Qt::UserRole + 2).toString() == "Directory") {   // For Directory
        selectedRecordID = index.data(Qt::UserRole).toString();
        selectedTreeItemIndex = index;
        directoryMoreActionMenu->exec(globalPos);
    } else {     // For Record
        selectedRecordID = index.data(Qt::UserRole).toString();
        selectedTreeItemIndex = index;
        recordMoreActionMenu->exec(globalPos);
    }
}

void ProjectPage::onTreeItemPlusActionClicked(const QModelIndex &index)
{
    QPoint globalPos = ui->treeView->mapToGlobal(QPoint(ui->treeView->visualRect(index).right()-80, ui->treeView->visualRect(index).bottom()));
    selectedRecordID = index.data(Qt::UserRole).toString();
    selectedTreeItemIndex = index;
    directoryPlusActionMenu->exec(globalPos);
}

void ProjectPage::showCreateRecordForm(QString parent, QString treeViewIndex)
{
    ui->createRecordForm->initializeForm(parent, treeViewIndex);
    ui->ViewPart->setCurrentWidget(ui->createRecordForm);
}

void ProjectPage::onRecordCreated(QString recID)
{
    RecordDataPresenter& presenter = RecordDataPresenter::instance();
    RecordDataPresenter::loadRecord(recID);

    Record record;
    record.id = presenter.recordID();
    record.title = presenter.title();
    record.type = presenter.type();
    record.parent = presenter.parent();
    record.status = presenter.status();
    record.deleted = presenter.deleted();
    record.treeViewIndex = presenter.treeViewIndex();

    QStandardItem *parentItem = nullptr;
    QStandardItem *item = createTreeItem(record);

    if (presenter.parent() == "PROJ") {
        parentItem = model.invisibleRootItem();
    } else {
        QModelIndexList matches = model.match(
            model.index(0, 0),             // start from root
            Qt::UserRole,              // dirID is stored here
            QVariant::fromValue(presenter.parent()),    // value to match
            1,                             // only one match
            Qt::MatchExactly | Qt::MatchRecursive
            );

        if (!matches.isEmpty()) {
            QModelIndex matchIndex = matches.first();
            if (matchIndex.isValid()) {
                parentItem=model.itemFromIndex(matchIndex);
            }
        }
    }

    if (parentItem == nullptr) {
        return;
    }

    if (presenter.treeViewIndex() == "") {
        parentItem->insertRow(0, item);
    } else {
        int insertRow = -1;
        for (int row=0; row<parentItem->rowCount(); row ++) {
            QStandardItem* childItem = parentItem->child(row);
            if (childItem->data(Qt::UserRole).toString() == record.treeViewIndex) {
                insertRow = row + 1;
                break;
            }

            if (childItem->data(Qt::UserRole + 3).toString() == record.id) {
                insertRow = row;
                break;
            }
        }

        if (insertRow != -1) {
            parentItem->insertRow(insertRow, item);
        } else {
            parentItem->appendRow(item);
        }
    }

    ui->treeView->expand(parentItem->index());
    ui->treeView->setCurrentIndex(item->index());
    ui->recordView->loadRecord(recID);
    ui->ViewPart->setCurrentWidget(ui->recordView);
}

void ProjectPage::onRecordUpdated(QString recID)
{
    ui->recordView->loadRecord(recID);

    RecordDataPresenter& presenter = RecordDataPresenter::instance();

    Record record;
    record.id = presenter.recordID();
    record.title = presenter.title();
    record.type = presenter.type();
    record.parent = presenter.parent();
    record.status = presenter.status();
    record.deleted = presenter.deleted();
    record.treeViewIndex = presenter.treeViewIndex();
    ui->ViewPart->setCurrentWidget(ui->recordView);

    QModelIndexList matches = model.match(
        model.index(0, 0),             // start from root
        Qt::UserRole,              // dirID is stored here
        QVariant::fromValue(recID),    // value to match
        1,                             // only one match
        Qt::MatchExactly | Qt::MatchRecursive
        );

    if (!matches.isEmpty()) {
        QModelIndex matchIndex = matches.first();
        QStandardItem *item = model.itemFromIndex(matchIndex);
        item->setText(record.title);

        QString iconPath = ProjectDataPresenter::getRecordIconPath(record);
        QString iconColor = "#1A1A1A";
        if (record.type == "Directory") {
            iconColor = "#1A1A1A";
        } else {
            if (record.status == "Draft") {
                iconColor = "#B82132";
            } else if (record.status == "Review") {
                iconColor = "#FEBA17";
            } else if (record.status == "Reviewed") {
                iconColor = "#3A7D44";
            }
        }

        QIcon icon = createColoredSvgIcon(iconPath, iconColor, QSize(16, 16));
        item->setIcon(QIcon(icon));
    }
}

void ProjectPage::onRecordDeleted(QString recID)
{
    QModelIndexList matches = model.match(
        model.index(0, 0),             // start from root
        Qt::UserRole,              // dirID is stored here
        QVariant::fromValue(recID),    // value to match
        1,                             // only one match
        Qt::MatchExactly | Qt::MatchRecursive
        );
    if (!matches.isEmpty()) {
        QModelIndex matchIndex = matches.first();

        QModelIndex parentIndex = matchIndex.parent();
        QStandardItem *parentItem = model.itemFromIndex(parentIndex);

        if (parentItem) {
            parentItem->removeRow(matchIndex.row());
        } else {
            model.removeRow(matchIndex.row());
        }
    }
}

void ProjectPage::onCancelEditRecord(QString recID)
{
    ui->ViewPart->setCurrentWidget(ui->recordView);
}

void ProjectPage::onSwitchEditRecordForm(QString recID)
{
    ui->editRecordForm->initializeForm(recID);
    ui->ViewPart->setCurrentWidget(ui->editRecordForm);
}
