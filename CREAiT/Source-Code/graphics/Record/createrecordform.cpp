#include "createrecordform.h"
#include "ui_createrecordform.h"
#include <QAbstractItemView>
#include <queue>
#include <recordtreeitem.h>
#include "Presenter/recorddatapresenter.h"
#include "Presenter/projectdatapresenter.h"

CreateRecordForm::CreateRecordForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CreateRecordForm)
{
    ui->setupUi(this);
    QAbstractItemView *popup = ui->statusFieldComboBox->view();
    popup->setFrameShape(QFrame::NoFrame);
    popup->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    popup->window()->setAttribute(Qt::WA_NoSystemBackground, true);
}

CreateRecordForm::~CreateRecordForm()
{
    delete ui;
}

void CreateRecordForm::initializeForm(const QString& parent, const QString& treeViewIndex)
{
    ui->titleFieldLineEdit->setText("");
    ui->versionFieldLineEdit->setText("1.0");

    QList<RecordType> typeList = RecordDataPresenter::getRecordTypeList();
    for (const RecordType &type : typeList) {
        ui->typeFieldComboBox->addItem(type.type, QVariant::fromValue(type.key));
    }

    m_parent = parent;
    m_treeViewIndex = treeViewIndex;

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

            if (!parentItem || parentID == "PROJ") parentItem = (RecordTreeItem*)model->invisibleRootItem();

            parentItem->appendRow(curItem);
        }

        q.pop();

        QList<Record> childList = projInstance.fetchChildRecordList(QString::fromStdString(currentID));
        // Push children to queue
        for (int i=0; i<childList.count(); i++) {
            q.push(childList[i].id.toStdString());
        }
    }

    ui->dirFieldCombo->setModel(model);
    ui->dirFieldCombo->setSelectedDirID(parent);
    // =============================================================

    ui->descTextEdit->setHtml("");

    ui->field1ValueEdit->setText("");
    ui->field1KeyEdit->setText("");
    ui->field2ValueEdit->setText("");
    ui->field2KeyEdit->setText("");
    ui->field3ValueEdit->setText("");
    ui->field3KeyEdit->setText("");
}

void CreateRecordForm::onSaveButtonClicked()
{
    QString recordType = ui->typeFieldComboBox->currentText();
    QString title = ui->titleFieldLineEdit->text();
    QString parent = ui->dirFieldCombo->selectedDirID();
    QString index = m_treeViewIndex;
    QString desc = ui->descTextEdit->html();

    ProjectDataPresenter &instance = ProjectDataPresenter::instance();
    instance.createRecord(recordType, title, parent, index, desc);
}
