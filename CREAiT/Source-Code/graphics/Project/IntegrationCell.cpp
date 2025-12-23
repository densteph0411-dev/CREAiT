#include "IntegrationCell.h"
#include "ui_IntegrationCell.h"
#include <QStyleOption>
#include <QPainter>
#include "projectsettingscontroller.h"

IntegrationCell::IntegrationCell(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IntegrationCell)
{
    ui->setupUi(this);
}

IntegrationCell::~IntegrationCell()
{
    delete ui;
}

void IntegrationCell::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this); // ✅ enables QSS drawing
}
