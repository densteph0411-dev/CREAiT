#include "automationcell.h"
#include <QMouseEvent>
#include <QPushButton>
#include <QStyle>
#include <QStyleOption>
#include <contextmenu.h>

AutomationCell::AutomationCell(int pageId, const QString& cellId, QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    this->pageId = pageId;
    this->cellId = cellId;
    connect(menuButton, &QPushButton::clicked, this, &AutomationCell::showMenu);
}

AutomationCell::~AutomationCell()
{

}

void AutomationCell::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void AutomationCell::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit requestEdit(pageId, cellId);
    }
}

void AutomationCell::showMenu() {
    ContextMenu* menu = new ContextMenu();

    connect(menu, &ContextMenu::requestEdit, this, &AutomationCell::edit);
    connect(menu, &ContextMenu::requestDelete, this, &AutomationCell::remove);

    QSize menuSize = menu->sizeHint();
    QPoint buttonPos = menuButton->mapToGlobal(QPoint(menuButton->width(), menuButton->height()));

    QPoint menuPos(buttonPos.x() - menuSize.width(), buttonPos.y());
    menu->exec(menuPos);
}

void AutomationCell::edit() {
    emit requestEdit(pageId, cellId);
}

void AutomationCell::remove() {
    emit requestDelete(pageId, cellId);
}
