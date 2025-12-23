#include "zoomsettingdialog.h"
#include "ui_zoomsettingdialog.h"
#include <QMessageBox>
#include <QProcess>
#include <QDir>
#include <QPainter>
#include <settings.h>

ZoomSettingDialog::ZoomSettingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ZoomSettingDialog)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    ui->setupUi(this);

    // First item = Auto (uses ComputeSnappedAppScale at next launch)
    ui->scaleComboBox->addItem("Auto (match display)", -1);
    // Fixed levels (percent)
    for (int pct : {75,100,125,150,175,200,225,250})
        ui->scaleComboBox->addItem(QString::number(pct) + "%", pct);

    // Load current
    const int saved = Settings::load("scalePercent", -1).toInt();
    int idx = 0;
    for (int i=0; i<ui->scaleComboBox->count(); i++) {
        if (ui->scaleComboBox->itemData(i).toInt()==saved) {
            idx = i;
            break;
        }
    }

    ui->scaleComboBox->setCurrentIndex(idx);

    connect(ui->cancelButton, &QPushButton::clicked, this, &ZoomSettingDialog::clickedCloseButton);
    connect(ui->closeButton, &QPushButton::clicked, this, &ZoomSettingDialog::clickedCloseButton);
    connect(ui->applyButton, &QPushButton::clicked, this, &ZoomSettingDialog::clickedApplyButton);
}

ZoomSettingDialog::~ZoomSettingDialog()
{
    delete ui;
}

void ZoomSettingDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void ZoomSettingDialog::showEvent(QShowEvent *event) {
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}

void ZoomSettingDialog::clickedCloseButton() {
    reject();
}

void ZoomSettingDialog::clickedApplyButton() {
    const int sel = ui->scaleComboBox->currentData().toInt();   // -1 or 75..250
    // Save to settings
    Settings::save("scalePercent", sel);

    // Relaunch self and exit
    const QString exe = QCoreApplication::applicationFilePath();
    const QStringList args = QCoreApplication::arguments();
    QProcess::startDetached(exe, args, QDir::currentPath());
    qApp->quit();

    accept();
}
