#include "integrationpanel.h"

IntegrationPanel::IntegrationPanel(int index, QWidget *parent)
    : QWidget(parent), index(index)
{
    setupUi(this);

    // Populate Integration dropdown
    toolSelect->clear();
    toolSelect->addItem("DOORs");
    toolSelect->addItem("JAMA");
    toolSelect->addItem("Polarion");
    toolSelect->addItem("None");

    connect(deleteButton, &QPushButton::clicked, this, &IntegrationPanel::clickedDeleteButton);
}

IntegrationPanel::~IntegrationPanel()
{

}

void IntegrationPanel::clickedDeleteButton() {
    emit requestDelete(index);
}
