#include "standardcell.h"
#include <QUrl>
#include <QFileInfo>
#include <QWidgetAction>
#include <QDesktopServices>
#include <QMouseEvent>
#include <contextmenu.h>


StandardCell::StandardCell(int id, QWidget *parent)
    : QWidget(parent), index(id) {
    setupUi(this);

    connect(imageContainer, &QPushButton::clicked, this, &StandardCell::open);
    connect(menuButton, &QPushButton::clicked, this, &StandardCell::showMenu);
}

StandardCell::~StandardCell() {

}

int StandardCell::cellId() {
    return index;
}

void StandardCell::setData(const QString& title, const QString& path) {
    titleLabel->setText(title);
    filePath = path;
}

void StandardCell::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QFileInfo fileInfo(filePath);
        if (fileInfo.exists() && fileInfo.isFile())
            QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }
}

void StandardCell::showMenu() {
    ContextMenu* menu = new ContextMenu();

    connect(menu, &ContextMenu::requestEdit, this, &StandardCell::edit);
    connect(menu, &ContextMenu::requestDelete, this, &StandardCell::remove);

    QSize menuSize = menu->sizeHint();
    QPoint buttonPos = menuButton->mapToGlobal(QPoint(menuButton->width(), menuButton->height()));

    QPoint menuPos(buttonPos.x() - menuSize.width(), buttonPos.y());
    menu->exec(menuPos);
}

void StandardCell::open() {
    QFileInfo fileInfo(filePath);
    if (fileInfo.exists() && fileInfo.isFile())
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void StandardCell::edit() {
    emit requestEdit(index);
}

void StandardCell::remove() {
    emit requestRemove(this);
}
