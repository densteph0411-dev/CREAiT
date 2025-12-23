#include "aimodelcell.h"
#include "ui_aimodelcell.h"
#include <QStyleOption>
#include <QPainter>
#include "projectsettingscontroller.h"

AIModelCell::AIModelCell(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AIModelCell)
{
    ui->setupUi(this);
}

AIModelCell::~AIModelCell()
{
    delete ui;
}

void AIModelCell::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this); // ✅ enables QSS drawing
}

void AIModelCell::setData(AIConnectionData data) {
    ui->connectedAITitle->setText(data.aiModel);
    ui->modelValue->setText(data.aiModel);
    ui->versionValue->setText(data.version);
}
