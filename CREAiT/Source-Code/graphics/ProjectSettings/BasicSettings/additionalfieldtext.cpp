#include "additionalfieldtext.h"

AdditionalFieldText::AdditionalFieldText(int id, QWidget *parent)
    : QWidget(parent), index(id)
{
    setupUi(this);

    connect(deleteButton, &QPushButton::clicked, this, &AdditionalFieldText::clickedDeleteButton);
}

AdditionalFieldText::~AdditionalFieldText() {

}

void AdditionalFieldText::clickedDeleteButton() {
    emit requestDelete(index);
}
