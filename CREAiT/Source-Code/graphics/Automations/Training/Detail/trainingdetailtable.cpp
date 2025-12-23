#include "trainingdetailtable.h"
#include "trainingdetailtablecell.h"
#include <QGridLayout>

TrainingDetailTable::TrainingDetailTable(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    TrainingDetailTableCell *header00 = new TrainingDetailTableCell(
        TrainingDetailTableCell::Header, {""}, {"top", "left"}, "", "border-top-left-radius: 6px;");
    TrainingDetailTableCell *header01 = new TrainingDetailTableCell(
        TrainingDetailTableCell::Header, {"Example Input"}, {"top", "left"});
    TrainingDetailTableCell *header02 = new TrainingDetailTableCell(
        TrainingDetailTableCell::Header, {"Example Result"}, {"top", "left", "right"}, "", "border-top-right-radius: 6px;");
    TrainingDetailTableCell *header10 = new TrainingDetailTableCell(
        TrainingDetailTableCell::Header, {"Dataset I"}, {"top", "left"});
    TrainingDetailTableCell *header20 = new TrainingDetailTableCell(
        TrainingDetailTableCell::Header, {"Dataset II"}, {"top", "left", "bottom"}, "", "border-bottom-left-radius: 6px;");
    TrainingDetailTableCell *cell11 = new TrainingDetailTableCell(
        TrainingDetailTableCell::Text, {"Lorem ipsum dolor sit amet..."}, {"top", "left"});
    TrainingDetailTableCell *cell12 = new TrainingDetailTableCell(
        TrainingDetailTableCell::File, {"Dataset_Kaggle.csv"}, {"top", "left", "right"}, ":/images/Automations/csv_icon.svg");
    TrainingDetailTableCell *cell21 = new TrainingDetailTableCell(
        TrainingDetailTableCell::Text, {"Lorem ipsum dolor sit amet..."}, {"top", "bottom", "left"});
    TrainingDetailTableCell *cell22 = new TrainingDetailTableCell(
        TrainingDetailTableCell::Records, {"Record 1", "Record 2", "Record 3"}, {"top", "left", "bottom", "right"},
        ":/images/Automations/record_icon.svg", "border-bottom-right-radius: 6px;");

    layout->addWidget(header00, 0, 0);
    layout->addWidget(header01, 0, 1);
    layout->addWidget(header02, 0, 2);
    layout->addWidget(header10, 1, 0);
    layout->addWidget(header20, 2, 0);
    layout->addWidget(cell11, 1, 1);
    layout->addWidget(cell12, 1, 2);
    layout->addWidget(cell21, 2, 1);
    layout->addWidget(cell22, 2, 2);

    layout->setColumnStretch(1, 1);
    layout->setColumnStretch(2, 1);
    layout->setRowStretch(1, 1);
    layout->setRowStretch(2, 1);
}

TrainingDetailTable::~TrainingDetailTable()
{

}
