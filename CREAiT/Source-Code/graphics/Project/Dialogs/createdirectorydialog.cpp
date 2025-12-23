#include "createdirectorydialog.h"
#include "ui_createdirectorydialog.h"
#include <QPainter>
#include <queue>
#include <recordtreeitem.h>
#include "Project/Presenter/projectdatapresenter.h"

CreateDirectoryDialog::CreateDirectoryDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateDirectoryDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    // ================== Set Data into TreeComboBox ==================
    std::map<std::string, RecordTreeItem*> recordItemMap;
    auto *model = new QStandardItemModel(this);

    ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();
    QList<Record> recordList = projInstance.fetchAllRecordList();

    // Step 1: Load all records into a map
    for (const auto &record : recordList) {
        RecordTreeItem *item = NULL;

        item = new RecordTreeItem(record.title, record.type);
        item->setData(record.id, Qt::UserRole);
        item->setData(record.parent, Qt::UserRole + 1);
        item->setData(record.type, Qt::UserRole + 2);
        item->setData(record.treeViewIndex, Qt::UserRole + 3);

        if (record.deleted == "true") {
            item->setData(true, Qt::UserRole + 4);
        } else {
            item->setData(false, Qt::UserRole + 4);
        }

        recordItemMap[record.id.toStdString()] = item;
    }

    // Step 2: Build tree hierarchy after all items are created

    std::string rootID = "PROJ";
    std::queue<std::string> q;
    q.push(rootID);

    while (!q.empty()) {
        std::string currentID = q.front();

        RecordTreeItem *curItem = recordItemMap[currentID];
        std::string parentID = curItem->data(Qt::UserRole + 1).toString().toStdString();
        std::string type = curItem->data(Qt::UserRole + 2).toString().toStdString();
        bool deleted = curItem->data(Qt::UserRole + 4).toBool();
        if (!deleted && type=="Directory") {
            RecordTreeItem *parentItem = nullptr;
            if (!parentID.empty() && recordItemMap.count(parentID)) {
                parentItem = recordItemMap[parentID];
            }

            if (!parentItem || parentID == "PROJ")
                parentItem = (RecordTreeItem*)model->invisibleRootItem();


            parentItem->appendRow(curItem);
        }

        q.pop();

        QList<Record> childList = projInstance.fetchChildRecordList(QString::fromStdString(currentID));
        // Push children to queue
        for (int i=0; i<childList.count(); i++) {
            q.push(childList[i].id.toStdString());
        }
    }

    ui->parentFieldCombo->setModel(model);
    // =============================================================
}

CreateDirectoryDialog::~CreateDirectoryDialog()
{
    delete ui;
}

void CreateDirectoryDialog::onCancelButtonClicked()
{
    reject();  // Closes dialog and returns QDialog::Rejected
}

void CreateDirectoryDialog::onCloseButtonClicked()
{
    reject();  // Closes dialog and returns QDialog::Rejected
}

void CreateDirectoryDialog::onCreateButtonClicked()
{
    m_dirName = ui->nameFieldEdit->text();
    m_parentID = ui->parentFieldCombo->selectedDirID();
    accept();
}

void CreateDirectoryDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));  // rgba = semi-transparent gray
}

void CreateDirectoryDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    printf("%s", mainWindow->objectName().toUtf8().data());
    printf("%d %d\n", parentRect.size().width(), parentRect.size().height());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}


QString CreateDirectoryDialog::directoryName() const {
    return m_dirName;
}

void CreateDirectoryDialog::setParentID(const QString& parentID) {
    m_parentID = parentID;

    ui->parentFieldCombo->setSelectedDirID(parentID);
}

QString CreateDirectoryDialog::parentID() const {
    return m_parentID;
}
