#include "versionitemwidget.h"
#include "ui_versionitemwidget.h"

VersionItemWidget::VersionItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VersionItemWidget)
{
    ui->setupUi(this);

    connect(ui->linkButton, &QPushButton::clicked, this, [this]() {
        emit openLinkClicked(versionString);
    });
}

VersionItemWidget::~VersionItemWidget()
{
    delete ui;
}

void VersionItemWidget::setData(const QString &version, const QString &description, const QString &date)
{
    versionString = version;
    ui->versionLabel->setText(version);
    ui->descLabel->setText(description);
    ui->dateLabel->setText(date);
}
