#include "hseparatorwidget.h"
#include "ui_hseparatorwidget.h"

HSeparatorWidget::HSeparatorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HSeparatorWidget)
{
    ui->setupUi(this);
}

HSeparatorWidget::~HSeparatorWidget()
{
    delete ui;
}
