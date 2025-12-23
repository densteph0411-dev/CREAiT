#include "reviewitemdetailtableheader.h"
#include "ui_reviewitemdetailtableheader.h"

ReviewItemDetailTableHeader::ReviewItemDetailTableHeader(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReviewItemDetailTableHeader)
{
    ui->setupUi(this);
}

ReviewItemDetailTableHeader::~ReviewItemDetailTableHeader()
{
    delete ui;
}
