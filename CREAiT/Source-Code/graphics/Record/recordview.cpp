#include "recordview.h"
#include "ui_recordview.h"
#include <QStandardItemModel>
#include <QScrollBar>
#include "versionhistorydialog.h"
#include "recordsummarydialog.h"
#include "recordaddlinkdialog.h"
#include "Presenter/recorddatapresenter.h"
#include <QMouseEvent>

RecordView::RecordView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecordView)
{
    ui->setupUi(this);

    // ----------------------- Init Additional Fields Table View ------------------------
    additionalFieldModel = new QStandardItemModel(this);
    additionalFieldModel->setColumnCount(3);
    additionalFieldModel->setHorizontalHeaderLabels({"Field Name", "Value", "Options"});

    QTableView *table = ui->additionalFieldsTableView;
    table->setModel(additionalFieldModel);
    table->verticalHeader()->setVisible(false);
    table->verticalHeader()->setDefaultSectionSize(60);
    table->setShowGrid(false);
    table->setAlternatingRowColors(false);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setFocusPolicy(Qt::NoFocus);
    table->setIconSize(QSize(20, 20));
    table->horizontalHeader()->setFixedHeight(46);
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->horizontalHeader()->setStretchLastSection(true);
    // Resize columns to content
    table->resizeColumnsToContents();
    // -------------------------------------------------------------------------------

    // ----------------------- Init Additional Fields Table View ------------------------
    linksModel = new QStandardItemModel(this);
    linksModel->setColumnCount(5);
    linksModel->setHorizontalHeaderLabels({"Link ID", "Title", "Relationship", "Type", "Options"});

    QTableView *linksTable = ui->linksTableView;
    linksTable->setModel(linksModel);
    linksTable->setShowGrid(false);
    linksTable->setAlternatingRowColors(false);
    linksTable->setSelectionMode(QAbstractItemView::NoSelection);
    linksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    linksTable->setFocusPolicy(Qt::NoFocus);
    linksTable->setIconSize(QSize(20, 20));
    linksTable->horizontalHeader()->setFixedHeight(46);
    linksTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    linksTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    linksTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    linksTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    linksTable->horizontalHeader()->setStretchLastSection(true);
    linksTable->verticalHeader()->setVisible(false);
    linksTable->verticalHeader()->setDefaultSectionSize(60);
    // Resize columns to content
    linksTable->resizeColumnsToContents();
    linksTable->viewport()->installEventFilter(this);
    // -------------------------------------------------------------------------------

    previewTimer.setSingleShot(true);
    previewTimer.setInterval(1000); // 500 ms delay, adjust as needed

    connect(&previewTimer, &QTimer::timeout, this, [this]() {
        if (pendingIndex.isValid()) {
            RecordSummaryDialog dlg(this, pendingRecID);
            dlg.exec();
        }
    });
}

RecordView::~RecordView()
{
    delete ui;
    delete additionalFieldModel;
    delete linksModel;
}

void RecordView::loadRecord(QString recID)
{
    currentRecordID = recID;

    RecordDataPresenter& presenter = RecordDataPresenter::instance();
    if (RecordDataPresenter::loadRecord(recID) == false)
        return;

    ui->recordIDLabel->setText("#" + recID);
    ui->recordTitleButton->setText(presenter.title());
    ui->typeLabel->setText(presenter.type());
    ui->lastModByLabel->setText(presenter.lastModifiedBy());
    ui->lastModOnLabel->setText(presenter.lastModifiedOn());
    ui->versionLabel->setText(presenter.version());
    ui->statusLabel->setText(presenter.status());
    ui->descTextBrowser->setOpenExternalLinks(true);
    ui->descTextBrowser->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->descTextBrowser->setHtml(presenter.description());

    additionalFieldModel->removeRows(0, additionalFieldModel->rowCount());
    QList<QString> keys = presenter.extraFieldKeys();
    for (const QString& key : keys) {
        QList<QStandardItem *> row;
        row << new QStandardItem(key);
        row << new QStandardItem(presenter.extraField(key).toString());

        QStandardItem *editItem = new QStandardItem();
        editItem->setIcon(QIcon(":/images/Record/edit-icon.svg"));  // use your icon
        editItem->setEditable(false);
        row << editItem;

        additionalFieldModel->appendRow(row);
    }

    QTableView *additionalFieldsTableView = ui->additionalFieldsTableView;
    additionalFieldsTableView->horizontalHeader()->setFixedHeight(46);
    additionalFieldsTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    additionalFieldsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    additionalFieldsTableView->horizontalHeader()->setStretchLastSection(true);

    linksModel->removeRows(0, linksModel->rowCount());
    for (const LinkRecord& link : presenter.links()) {
        QList<QStandardItem*> row;
        row << new QStandardItem(link.linkID)
            << new QStandardItem(link.title)
            << new QStandardItem(link.relationship)
            << new QStandardItem(link.type);

        QStandardItem *editItem = new QStandardItem();
        editItem->setIcon(QIcon(":/images/Record/edit-icon.svg"));  // use your icon
        editItem->setEditable(false);
        row << editItem;

        linksModel->appendRow(row);
    }

    QTableView *linksTable = ui->linksTableView;
    linksTable->horizontalHeader()->setFixedHeight(46);
    linksTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    linksTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    linksTable->horizontalHeader()->setStretchLastSection(true);
}

void RecordView::onVersionHistoryButtonClicked()
{
    VersionHistoryDialog dlg(this);
    dlg.exec();
}

void RecordView::onRecordLinkButtonClicked()
{
    RecordSummaryDialog dlg(this);
    dlg.exec();
}

void RecordView::onEditRecordButtonClicked()
{
    emit switchToEditRecordView(currentRecordID);
}

void RecordView::onAddLinkButtonClicked()
{
    RecordAddLinkDialog dlg(currentRecordID, this);
    if (dlg.exec() == QDialog::Accepted)
    {
        loadRecord(currentRecordID);
    }
}

void RecordView::onMouseHoverLinkTableItem(QModelIndex index)
{
    if (index.isValid() && index.column() == 0) {
        QString recordID = linksModel->data(index, 0).toString();

        if (pendingRecID != recordID) {
            if (previewTimer.isActive())
            {
                previewTimer.stop();
            }

            pendingIndex = index;
            pendingRecID = linksModel->data(index, 0).toString();

            previewTimer.start();
        }
    } else {
        // hideHoverPopup();
    }
}

bool RecordView::eventFilter(QObject *obj, QEvent *event)
{
    static QString shownRecID = "";

    if (obj == ui->linksTableView->viewport()) {
        if (event->type() == QEvent::MouseMove) {
            auto *mouseEvent = static_cast<QMouseEvent *>(event);
            QModelIndex index = ui->linksTableView->indexAt(mouseEvent->pos());
            if (index.isValid() && index.column() == 0) {
                QString recID = linksModel->data(index, 0).toString();

                // If moving over a new record, reset timer
                if (recID != shownRecID) {
                    pendingRecID = recID;
                    pendingIndex = index;
                    previewTimer.start(); // restart delay timer

                    shownRecID = recID;
                }
            } else {
                // Mouse over invalid area – cancel timer and hide popover
                previewTimer.stop();
                shownRecID = "";
                pendingIndex = QModelIndex();
            }
        } else if (event->type() == QEvent::Leave) {
            previewTimer.stop();
            shownRecID = "";
            pendingIndex = QModelIndex();
        }
    }

    return QWidget::eventFilter(obj, event);
}
