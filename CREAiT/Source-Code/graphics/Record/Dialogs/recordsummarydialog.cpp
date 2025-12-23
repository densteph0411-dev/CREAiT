#include "recordsummarydialog.h"
#include "ui_recordsummarydialog.h"
#include <QPainter>
#include "Presenter/recorddatapresenter.h"

RecordSummaryDialog::RecordSummaryDialog(QWidget *parent, const QString& recID)
    : QDialog(parent)
    , ui(new Ui::RecordSummaryDialog), m_recordID(recID)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

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

RecordSummaryDialog::~RecordSummaryDialog()
{
    delete ui;
}

void RecordSummaryDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));  // rgba = semi-transparent gray
}

void RecordSummaryDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}

void RecordSummaryDialog::onCloseButtonClicked()
{
    accept();  // Closes dialog
}
