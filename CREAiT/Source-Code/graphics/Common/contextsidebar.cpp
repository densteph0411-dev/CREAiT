#include "contextsidebar.h"
#include <QButtonGroup>

ContextSidebar::ContextSidebar(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);

    // auto setButtonProperties = [&](QPushButton* button, const QString& text, const QString& icon, int id) {
    //     button->setIcon(QIcon(icon));
    //     button->setIconSize(QSize(20, 20));
    //     button->setText(text);
    //     button->setCheckable(true);
    //     button->setContentsMargins(10, 10, 10, 10);
    //     buttonGroup->addButton(button, id);
    //     return button;
    // };

    // setButtonProperties(firstButton, "Basic Settings",  ":/images/ProjectSettings/basic_settings.svg", 0);
    // setButtonProperties(secondButton, "Integrations",  ":/images/ProjectSettings/integrations.svg", 1);
    // setButtonProperties(thirdButton, "AI Settings",  ":/images/ProjectSettings/ai_settings.svg", 2);
    // setButtonProperties(fourthButton, "User Management",  ":/images/ProjectSettings/user_management.svg", 3);

    pageButtons = new QList<QPushButton*>();
    for (auto button : { firstButton, secondButton, thirdButton, fourthButton }) {
        pageButtons->append(button);
    }

    for (int i = 0; i < pageButtons->size(); ++i) {
        connect(pageButtons->at(i), &QPushButton::clicked, this, [this, i]() {
            emit requestPage(i);
        });
    }

}

ContextSidebar::~ContextSidebar()
{

}

void ContextSidebar::setItemData(int id, const QString& title, const QString& iconPath, bool hide) {
    QPushButton* button = pageButtons->at(id);
    button->setIcon(QIcon(iconPath));
    button->setIconSize(QSize(20, 20));
    button->setText(title);
    button->setCheckable(true);
    button->setContentsMargins(10, 10, 10, 10);
    buttonGroup->addButton(button, id);

    if (hide) button->hide();
}

void ContextSidebar::handleButtonSelect(int buttonIndex) {
    if (buttonIndex < 0 || buttonIndex >= pageButtons->size()) {
        qDebug() << "Invalid button index:" << buttonIndex;
        return;
    }

    for (int i = 0; i < pageButtons->size(); ++i) {
        pageButtons->at(i)->setChecked(i == buttonIndex);
    }
}
