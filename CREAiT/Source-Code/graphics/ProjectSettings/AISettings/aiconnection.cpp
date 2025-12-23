#include "aiconnection.h"
#include <QStyle>
#include <QStyleOption>
#include <QMouseEvent>
#include <QPainter>
#include <contextmenu.h>

AIConnection::AIConnection(const int pageId, const int cellId, QWidget *parent)
    : QWidget(parent), pageId(pageId), cellId(cellId)
{
    setupUi(this);

    connect(refreshButton, &QPushButton::clicked, this, &AIConnection::refresh);
    connect(menuButton, &QPushButton::clicked, this, &AIConnection::showMenu);
}

AIConnection::~AIConnection()
{

}

void AIConnection::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void AIConnection::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit requestEdit(pageId, cellId);
    }
}

void AIConnection::refresh() {
    emit requestRefresh(pageId, cellId);
}

void AIConnection::showMenu() {
    ContextMenu* menu = new ContextMenu();

    connect(menu, &ContextMenu::requestEdit, this, &AIConnection::edit);
    connect(menu, &ContextMenu::requestDelete, this, &AIConnection::remove);

    QSize menuSize = menu->sizeHint();
    QPoint buttonPos = menuButton->mapToGlobal(QPoint(menuButton->width(), menuButton->height()));

    QPoint menuPos(buttonPos.x() - menuSize.width(), buttonPos.y());
    menu->exec(menuPos);
}

void AIConnection::edit() {
    emit requestEdit(pageId, cellId);
}

void AIConnection::remove() {
    emit requestDelete(pageId, cellId);
}

