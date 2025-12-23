#include <QtWidgets>
#include "flowlayout.h"

FlowLayout::FlowLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing) {
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::~FlowLayout() {
    QLayoutItem* item = nullptr;

    while ((item = takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        if (widget) {
            widget->setParent(nullptr);
            widget->deleteLater();
        }
        delete item;
    }
}


void FlowLayout::addItem(QLayoutItem *item) {
    itemList.append(item);
}

int FlowLayout::horizontalSpacing() const {
    if (m_hSpace >= 0) {
        return m_hSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int FlowLayout::verticalSpacing() const {
    if (m_vSpace >= 0) {
        return m_vSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

int FlowLayout::count() const {
    return itemList.size();
}

QLayoutItem *FlowLayout::itemAt(int index) const {
    return itemList.value(index);
}

QLayoutItem *FlowLayout::takeAt(int index) {
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    return nullptr;
}

Qt::Orientations FlowLayout::expandingDirections() const {
    return {};
}

bool FlowLayout::hasHeightForWidth() const {
    return true;
}

int FlowLayout::heightForWidth(int width) const {
    return doLayout(QRect(0, 0, width, 0), true);
}

QSize FlowLayout::sizeHint() const {
    return minimumSize();
}

QSize FlowLayout::minimumSize() const {
    QSize size;
    for (QLayoutItem *item : itemList)
        size = size.expandedTo(item->minimumSize());

    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    size += QSize(left + right, top + bottom);
    return size;
}

void FlowLayout::setGeometry(const QRect &rect) {
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

int FlowLayout::doLayout(const QRect &rect, bool testOnly) const {
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);

    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;
    //! [9]

    for (QLayoutItem *item : itemList) {
        QWidget *widget = item->widget();
        int spaceX = horizontalSpacing();
        if (spaceX == -1)
            spaceX = widget->style()->layoutSpacing(
                QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
        int spaceY = verticalSpacing();
        if (spaceX == -1) spaceX = smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
        if (spaceY == -1) spaceY = smartSpacing(QStyle::PM_LayoutVerticalSpacing);

        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
            x = effectiveRect.x();
            y += lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly)
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

        x = nextX;
        lineHeight = std::max(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - effectiveRect.y() + bottom;
}

int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const {
    const QWidget *parentWidget = nullptr;
    if (const QObject *parentObj = parent()) {
        parentWidget = qobject_cast<const QWidget *>(parentObj);
    }

    if (parentWidget) {
        return parentWidget->style()->pixelMetric(pm, nullptr, parentWidget);
    }
    return -1;
}

