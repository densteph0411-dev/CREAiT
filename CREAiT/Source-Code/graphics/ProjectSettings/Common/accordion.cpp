#include "accordion.h"

Accordion::Accordion(QWidget *parent)
    : QWidget(parent), expanded(true)
{
    setupUi(this);

    ;

    updateStyle();
    connect(toggleButton, &QPushButton::clicked, this, &Accordion::clickedToggleButton);
}

Accordion::~Accordion()
{

}

void Accordion::clickedToggleButton() {
    expanded = !expanded;
    contentArea->setVisible(expanded);
    toggleButton->setIcon(QIcon(expanded ? ":images/chevron-up.svg" : ":images/chevron-down.svg"));

    updateStyle();
}

void Accordion::setTitle(const QString& title) {
    headerLabel->setText(title);
}


void Accordion::updateStyle() {
    QString borderRadius = expanded ?
                               "border-top-left-radius: 8px; border-top-right-radius: 8px;" :
                               "border-radius: 8px;";

    QString headerStyle = QString(
                              "#header { background-color: white; border: 1px solid #34343426; %1 } "
                              ).arg(borderRadius);

    header->setStyleSheet(headerStyle);
}

