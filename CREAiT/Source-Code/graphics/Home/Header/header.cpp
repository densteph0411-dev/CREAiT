#include "header.h"
#include <Presenter/projectdatapresenter.h>

Header::Header(QWidget *parent)
    : QWidget(parent)

{
   setupUi(this);

    // Connect signals from WindowControlBar to the Header
    connect(windowControlBar, &WindowControlBar::handleAIChat, this, &Header::handleAIChat);
    connect(windowControlBar, &WindowControlBar::handleZoom, this, &Header::handleZoom);
    connect(windowControlBar, &WindowControlBar::handleUser, this, &Header::handleUser);

    connect(addressBar, &AddressBar::activatedItem, this, &Header::handleTabNavigation);
    connect(addressBar, &AddressBar::closedItem, this, &Header::handleAddressBarItemClose);

    addressBar->addItem(QIcon(":/images/header-home.svg"), "Home", false);
}

Header::~Header()
{

}

void Header::addItemtoAddressBar(const QString& projectName) {
    addressBar->addItem(QIcon(":/images/header-project.svg"), projectName, true);
}
