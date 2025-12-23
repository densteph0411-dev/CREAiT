#include "recordaddlinkdialog.h"
#include "ui_recordaddlinkdialog.h"
#include "linkrecordtablemodel.h"
#include <QPainter>
#include <QMouseEvent>
#include "Presenter/projectdatapresenter.h"

RecordAddLinkDialog::RecordAddLinkDialog(QString recID, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RecordAddLinkDialog), m_recordID(recID)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    model = new LinkRecordTableModel();

    ui->recordTableView->setModel(model);
    ui->recordTableView->setShowGrid(false);
    ui->recordTableView->setAlternatingRowColors(false);
    ui->recordTableView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->recordTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->recordTableView->setFocusPolicy(Qt::NoFocus);
    ui->recordTableView->horizontalHeader()->setFixedHeight(46);
    ui->recordTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->recordTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->recordTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->recordTableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->recordTableView->verticalHeader()->setDefaultSectionSize(46);

    ui->recordTableView->resizeColumnsToContents();

    ui->recordTableView->viewport()->installEventFilter(this);

    QList<LinkType> typeList = RecordDataPresenter::getLinkTypeList();
    ui->linkTypeFieldCombo->clear();

    for (const LinkType &type : typeList) {
        ui->linkTypeFieldCombo->addItem(type.key, QVariant::fromValue(type.key));
    }

    stepIndex = 0;
    ui->step1Check->setChecked(true);
    ui->step2Check->setChecked(false);
    ui->step1Group->setVisible(true);
    ui->step2Group->setVisible(false);

    searchTimer.setSingleShot(true);  // only fires once per start
    connect(&searchTimer, &QTimer::timeout, this, [=]() {
        QString keyword = ui->searchLineEdit->text();
        onSearchChanged(keyword);  // your actual search function
    });

    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, [=](const QString &text) {
        searchTimer.start(1000);  // 1000 ms debounce delay
    });

    previewTimer.setSingleShot(true);
    previewTimer.setInterval(1000); // 500 ms delay, adjust as needed

    connect(&previewTimer, &QTimer::timeout, this, [this]() {
        if (pendingIndex.isValid()) {
            QRect rect = ui->recordTableView->visualRect(pendingIndex);
            QPoint globalPos = ui->recordTableView->viewport()->mapTo(this, rect.bottomLeft());

            auto *popover = RecordSummaryPopover::instance();
            popover->loadRecord(pendingRecID);
            popover->showAt(globalPos);
        }
    });
}

RecordAddLinkDialog::~RecordAddLinkDialog()
{
    delete ui;
    delete model;
}

void RecordAddLinkDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));  // rgba = semi-transparent gray
}

void RecordAddLinkDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}

void RecordAddLinkDialog::onCloseButtonClicked()
{
    reject();
}

void RecordAddLinkDialog::onNextButtonClicked()
{
    if ( stepIndex == 0) {
        QVector<LinkRecordItem> checkedItems = model->checkedItems();

        if (checkedItems.count()> 0) {
            ui->step1Check->setChecked(true);
            ui->step2Check->setChecked(true);
            stepIndex ++;
            ui->step1Group->setVisible(false);
            ui->step2Group->setVisible(true);
        }
    } else if (stepIndex == 1) {
        QVector<LinkRecordItem> checkedItems = model->checkedItems();

        RecordDataPresenter::loadRecord(m_recordID);
        RecordDataPresenter& presenter = RecordDataPresenter::instance();

        QString linkType = ui->linkTypeFieldCombo->currentText();
        QString relationship = ui->relationshipFieldCombo->currentText();

        for (const LinkRecordItem& item : checkedItems) {
            LinkRecord linkRecord;
            linkRecord.linkID = item.id;
            linkRecord.title = item.title;
            linkRecord.type = linkType;
            linkRecord.relationship = relationship;

            presenter.addLink(linkRecord, true);
        }

        accept();
    }
}

void RecordAddLinkDialog::onSearchChanged(QString keyword)
{
    if (keyword.isEmpty())
    {
        model->resetData({});
    } else {
        ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();
        QList<Record> records = projInstance.searchRecordsByKeyword(keyword);

        QVector<LinkRecordItem> items;
        for (const Record &rec : records) {
            items.append(LinkRecordItem{
                false,          // not checked by default
                rec.id,
                rec.title,
                rec.type
            });
        }
        model->resetData(items);
    }
}

bool RecordAddLinkDialog::eventFilter(QObject *obj, QEvent *event)
{
    static QString shownRecID = "";

    if (obj == ui->recordTableView->viewport()) {
        if (event->type() == QEvent::MouseMove) {
            auto *mouseEvent = static_cast<QMouseEvent *>(event);
            QModelIndex index = ui->recordTableView->indexAt(mouseEvent->pos());
            if (index.isValid() && index.column() == 1) {
                QString recID = index.data().toString();

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
                RecordSummaryPopover::instance()->hidePopover();
                shownRecID = "";
                pendingIndex = QModelIndex();
            }
        } else if (event->type() == QEvent::Leave) {
            previewTimer.stop();
            RecordSummaryPopover::instance()->hidePopover();
            shownRecID = "";
            pendingIndex = QModelIndex();
        }
        qDebug() << "Qevent Type:" << event->type();
    }

    return QDialog::eventFilter(obj, event);
}
