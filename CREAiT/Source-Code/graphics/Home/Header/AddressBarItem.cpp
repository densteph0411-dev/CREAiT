
#include "addressbaritem.h"
#include <QStyle>

AddressBarItem::AddressBarItem(int id, const QIcon& icon, const QString& text, bool closable, QWidget *parent)
    : QWidget(parent), index(id), name(text), isActive(false), showClose(closable)

{
    setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    iconLabel->setPixmap(icon.pixmap(20, 20));
    textLabel->setText(text);
    closeButton->setCursor(Qt::PointingHandCursor);

    if (!showClose) {
        mainLayout->removeWidget(closeButton);
        closeButton->hide();
    }

    setActive(false);

    connect(closeButton, &QToolButton::clicked, this, &AddressBarItem::clickedClose);

    setStyleSheet(loadStyleSheet(":/stylesheets/Home/AddressBarItem.qss"));
}

AddressBarItem::~AddressBarItem() {
}

void AddressBarItem::setActive(bool active) {
    isActive = active;
    itemContainer->setProperty("active", active);

    // Optional: trigger style refresh
    style()->unpolish(itemContainer);
    style()->polish(itemContainer);
}

bool AddressBarItem::active() const {
    return isActive;
}

void AddressBarItem::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(index, name);
    }

    QWidget::mousePressEvent(event);
}

void AddressBarItem::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) return;

    QWidget::mouseDoubleClickEvent(event);
}


void AddressBarItem::clickedClose() {
    emit requestedClose(index, name);
}
