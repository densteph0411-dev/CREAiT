#include "additionalfielditem.h"

AdditionalFieldItem::AdditionalFieldItem(int id, QWidget *parent)
    : QWidget(parent), index(id)
{
    setupUi(this);

    fieldNameEdit->setPlaceholderText("Enter field name here");

    connect(deleteButton, &QPushButton::clicked, this, &AdditionalFieldItem::clickedDeleteButton);
}

AdditionalFieldItem::~AdditionalFieldItem()
{

}

void AdditionalFieldItem::clickedDeleteButton() {
    emit requestDelete(index);
}
