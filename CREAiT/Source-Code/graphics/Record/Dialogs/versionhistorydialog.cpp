#include "versionhistorydialog.h"
#include "ui_versionhistorydialog.h"
#include "versionitemwidget.h"
#include <QPainter>
#include "Presenter/recorddatapresenter.h"

VersionHistoryDialog::VersionHistoryDialog(QWidget *parent, const QString& recID)
    : QDialog(parent)
    , ui(new Ui::VersionHistoryDialog), m_recordID(recID)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    RecordDataPresenter& presenter = RecordDataPresenter::instance();
    RecordDataPresenter::loadRecord(recID);

    const QList<VersionHistory>& versions = presenter.versions();

    QListWidget *listWidget = ui->historyListWidget;

    listWidget->setSpacing(8);

    for (const VersionHistory &v : versions) {
        QListWidgetItem *item = new QListWidgetItem(listWidget);
        VersionItemWidget *widget = new VersionItemWidget;
        widget->setData(v.number, v.description, v.date);

        QObject::connect(widget, &VersionItemWidget::openLinkClicked, [](const QString &version){
            qDebug() << "Open link clicked for:" << version;
        });

        item->setSizeHint(widget->sizeHint());
        listWidget->addItem(item);
        listWidget->setItemWidget(item, widget);
    }
}

VersionHistoryDialog::~VersionHistoryDialog()
{
    delete ui;
}

void VersionHistoryDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));  // rgba = semi-transparent gray
}

void VersionHistoryDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}

void VersionHistoryDialog::onCloseButtonClicked()
{
    accept();  // Closes dialog
}
