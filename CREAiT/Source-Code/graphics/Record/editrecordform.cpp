#include "editrecordform.h"
#include "ui_editrecordform.h"
#include "Presenter/recorddatapresenter.h"
#include "Presenter/projectdatapresenter.h"
#include <QStandardItem>
#include <queue>
#include <recordtreeitem.h>

EditRecordForm::EditRecordForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditRecordForm)
{
    ui->setupUi(this);
}

EditRecordForm::~EditRecordForm()
{
    delete ui;
}

void EditRecordForm::initializeForm(QString recID)
{
    recordID = recID;

    RecordDataPresenter& presenter = RecordDataPresenter::instance();
    RecordDataPresenter::loadRecord(recID);

    ui->titleFieldLineEdit->setText(presenter.title());
    ui->versionFieldLineEdit->setText(presenter.version());

    ui->typeFieldComboBox->clear();
    QList<RecordType> typeList = RecordDataPresenter::getRecordTypeList();
    for (const RecordType &type : typeList) {
        ui->typeFieldComboBox->addItem(type.type, QVariant::fromValue(type.key));
    }

    int index = ui->typeFieldComboBox->findText(presenter.type());
    if (index != -1) {
        ui->typeFieldComboBox->setCurrentIndex(index);
    }

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

    ui->dirFieldCombo->setSelectedDirID(presenter.parent());
    // =============================================================

    ui->descTextEdit->setHtml(presenter.description());
    ui->statusFieldComboBox->setCurrentText(presenter.status());
}

void EditRecordForm::onSaveButtonClicked()
{
    QString type = ui->typeFieldComboBox->currentText();
    QString title = ui->titleFieldLineEdit->text();
    QString version = ui->versionFieldLineEdit->text();
    QString parent = ui->dirFieldCombo->selectedDirID();
    QString index = "";
    QString desc = ui->descTextEdit->html();
    QString status = ui->statusFieldComboBox->currentText();

    RecordDataPresenter& presenter = RecordDataPresenter::instance();
    RecordDataPresenter::loadRecord(recordID);

    presenter.setTitle(title);
    presenter.setType(type);
    presenter.setVersion(version);
    presenter.setDescription(desc);
    presenter.setParent(parent);
    presenter.setStatus(status);

    // presenter.setLastModifiedBy("");
    // presenter.setLastModifiedOn("");

    presenter.updateRecord();

    // presenter.setExtraField();


    emit recordUpdated(recordID);
}


void EditRecordForm::onCancelButtonClicked()
{
    emit cancelEditRecord(recordID);
}

