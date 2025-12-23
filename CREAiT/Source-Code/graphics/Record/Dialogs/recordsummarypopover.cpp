#include "recordsummarypopover.h"
#include "ui_recordsummarypopover.h"
#include <QGraphicsDropShadowEffect>
#include <Presenter/recorddatapresenter.h>
#include <QTimer>

RecordSummaryPopover* RecordSummaryPopover::s_instance = nullptr;

RecordSummaryPopover::RecordSummaryPopover(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecordSummaryPopover)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    this->setAttribute(Qt::WA_TranslucentBackground);  // important for shadow rendering
    this->setAttribute(Qt::WA_ShowWithoutActivating);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 80));  // soft semi-transparent black
    ui->contentWrapper->setGraphicsEffect(shadow);

    this->setMouseTracking(true);
}

RecordSummaryPopover::~RecordSummaryPopover()
{
    delete ui;
}

RecordSummaryPopover* RecordSummaryPopover::instance(QWidget *parent)
{
    if (!s_instance)
        s_instance = new RecordSummaryPopover(parent);
    return s_instance;
}

void RecordSummaryPopover::loadRecord(const QString &recID)
{
    RecordDataPresenter::loadRecord(recID);
    RecordDataPresenter& presenter = RecordDataPresenter::instance();

    ui->dialogTitle->setText(presenter.title());
    ui->IdLabel->setText(presenter.recordID());
    ui->typeFieldLabel->setText(presenter.type());
    ui->versionFieldLabel->setText(presenter.version());
    ui->statusValueLabel->setText(presenter.status());
    ui->descTextView->setOpenExternalLinks(true);
    ui->descTextView->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->descTextView->setHtml(presenter.description());
}

void RecordSummaryPopover::showAt(const QPoint &globalPos)
{
    move(globalPos);
    show();
}

void RecordSummaryPopover::hidePopover()
{
    hide();
}

void RecordSummaryPopover::hideIfNotHovered()
{
    // Delay the check slightly (e.g., 100ms) to allow hover move into the popover
    QTimer::singleShot(100, this, [this]() {
        if (!this->geometry().contains(QCursor::pos())) {
            this->hide();
        }
    });
}

void RecordSummaryPopover::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "PopOver Leave";
    hide();
}
