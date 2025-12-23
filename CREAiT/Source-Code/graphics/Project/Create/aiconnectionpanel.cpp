#include "aiconnectionpanel.h"

AIConnectionPanel::AIConnectionPanel(int id, QWidget *parent)
    : QWidget(parent), index(id)
{
    setupUi(this);

    // Populate AI Connection dropdown
    aiSelect->clear();
    aiSelect->addItem("OpenAI");
    aiSelect->addItem("ACE");

    connect(deleteButton, &QPushButton::clicked, this, &AIConnectionPanel::clickedDeleteButton);
}

AIConnectionPanel::~AIConnectionPanel()
{

}

void AIConnectionPanel::clickedDeleteButton() {
    emit requestDelete(index);
}
