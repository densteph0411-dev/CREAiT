#include "sidebar.h"

#include <QButtonGroup>
#include <QList>

SideBar::SideBar(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
    mainLayout->setAlignment(Qt::AlignTop);

    QPixmap pixmap(":/images/logo.png");

    QButtonGroup *buttonGroup = new QButtonGroup(this);

    buttonGroup->setExclusive(true);

    auto setButtonProperties = [&](QToolButton* button, const QString& text, const QString& icon, int id) {
        button->setText(text);
        button->setIcon(QIcon(icon));
        button->setIconSize(QSize(38, 38));
        button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        button->setCheckable(true);
        buttonGroup->addButton(button, id);
        return button;
    };

    setButtonProperties(projectsButton, "Projects", ":/images/sidebar_projects.png", PROJECTS);
    setButtonProperties(documentsButton, "Documents", ":/images/sidebar_documents.png", DOCUMENTS);
    setButtonProperties(reviewsButton, "Reviews", ":/images/sidebar_reviews.png", REVIEWS);
    setButtonProperties(automationsButton, "Automations", ":/images/sidebar_automations.png", AUTOMATIONS);
    setButtonProperties(projectSettingsButton, "Project Settings", ":/images/sidebar_projectsettings.png", PROJECT_SETTINGS);

    pageButtons = new QList<QToolButton*>();

    for (auto button : { projectsButton, documentsButton, reviewsButton, automationsButton, projectSettingsButton }) {
        pageButtons->append(button);
    }

    for (int i = 0; i < pageButtons->size(); ++i) {
        connect(pageButtons->at(i), &QToolButton::clicked, this, [this, i]() {
            emit requestedPage(i);
        });
    }

    setStyleSheet(loadStyleSheet(":/stylesheets/Home/SideBar.qss"));
}

SideBar::~SideBar() {

}

void SideBar::selectedButton(int buttonIndex) {
    if (buttonIndex < 0 || buttonIndex >= pageButtons->size()) return;

    pageButtons->at(buttonIndex)->setChecked(true);
}
