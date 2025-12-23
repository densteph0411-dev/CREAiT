#include "integration.h"
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <contextmenu.h>

Integration::Integration(int pageId, int cellId, QWidget *parent)
    : QWidget(parent), pageId(pageId), cellId(cellId)
{
    setupUi(this);

    connect(refreshButton, &QPushButton::clicked, this, &Integration::refresh);
    connect(menuButton, &QPushButton::clicked, this, &Integration::showMenu);
}

Integration::~Integration()
{

}

void Integration::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Integration::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit requestEdit(pageId, cellId);
    }
}

void Integration::refresh() {
    emit requestRefresh(pageId, cellId);
}

void Integration::showMenu() {
    ContextMenu* menu = new ContextMenu();

    connect(menu, &ContextMenu::requestEdit, this, &Integration::edit);
    connect(menu, &ContextMenu::requestDelete, this, &Integration::remove);

    QSize menuSize = menu->sizeHint();
    QPoint buttonPos = menuButton->mapToGlobal(QPoint(menuButton->width(), menuButton->height()));

    QPoint menuPos(buttonPos.x() - menuSize.width(), buttonPos.y());
    menu->exec(menuPos);
}

int Integration::getCellId() {
    return cellId;
}

void Integration::edit() {
    emit requestEdit(pageId, cellId);
}

void Integration::remove() {
    emit requestDelete(pageId, cellId);
}
