#include "additionalfieldlabel.h"

AdditionalFieldLabel::AdditionalFieldLabel(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
}

AdditionalFieldLabel::~AdditionalFieldLabel() {

}

void AdditionalFieldLabel::setData(const QString& property, const QString& value) {
    propertyLabel->setText(property);
    valueLabel->setText(value);
}
