#include "projectcard.h"
#include <contextmenu.h>

ProjectCard::ProjectCard(const QString& name, QWidget *parent)
    : QWidget(parent), projectName(name)
{
    setupUi(this);

    projectNameLabel->setText(projectName);
    projectInfo->setText("Lorem Ipsum ath etr fkg pot ejffm ref hyu...");

    connect(menuButton, &QToolButton::clicked, this, &ProjectCard::showMenu);
}

ProjectCard::~ProjectCard() {

}

void ProjectCard::mouseDoubleClickEvent(QMouseEvent *event) {
    emit requestProjectView(projectName);
}

void ProjectCard::showMenu() {
    ContextMenu* menu = new ContextMenu();

    connect(menu, &ContextMenu::requestEdit, this, &ProjectCard::edit);
    connect(menu, &ContextMenu::requestDelete, this, &ProjectCard::remove);

    QSize menuSize = menu->sizeHint();
    QPoint buttonPos = menuButton->mapToGlobal(QPoint(menuButton->width(), menuButton->height()));

    QPoint menuPos(buttonPos.x() - menuSize.width(), buttonPos.y());
    menu->exec(menuPos);
}

void ProjectCard::edit() {
    emit requestProjectView(projectName);
}

void ProjectCard::remove() {
    emit requestProjectDelete(this);
}
