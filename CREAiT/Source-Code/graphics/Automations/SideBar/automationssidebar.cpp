#include <QButtonGroup>

#include "automationssidebar.h"

AutomationsSideBar::AutomationsSideBar(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);

    auto setButtonProperties = [&](QToolButton* button, const QString& icon, int id) {
        button->setIcon(QIcon(icon));
        button->setIconSize(QSize(20, 20));
        button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        button->setCheckable(true);
        button->setContentsMargins(10, 10, 10, 10);
        buttonGroup->addButton(button, id);
        return button;
    };

    setButtonProperties(automationsButton, ":/images/Automations/sidebar_automations.png", 0);
    setButtonProperties(trainingsButton, ":/images/Automations/sidebar_trainings.png", 1);

    pageButtons = new QList<QToolButton*>();

    for (auto button : { automationsButton, trainingsButton }) {
        pageButtons->append(button);
    }

    for (int i = 0; i < pageButtons->size(); ++i) {
        connect(pageButtons->at(i), &QToolButton::clicked, this, [this, i]() {
            emit requestedPage(i);
        });
    }

    setStyleSheet(loadStyleSheet(":/stylesheets/Automations/AutomationsSideBar.qss"));
}

AutomationsSideBar::~AutomationsSideBar()
{
}

void AutomationsSideBar::handleButtonSelect(int buttonIndex) {
    if (buttonIndex < 0 || buttonIndex >= pageButtons->size()) {
        qDebug() << "Invalid button index:" << buttonIndex;
        return;
    }

    pageButtons->at(buttonIndex)->setChecked(true);
}
