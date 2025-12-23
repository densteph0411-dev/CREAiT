#ifndef PROJECTTREEDELEGATE_H
#define PROJECTTREEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QAbstractItemView>
#include <QIcon>

class ProjectTreeDelegate : public QStyledItemDelegate {
    Q_OBJECT
    mutable QModelIndex hoveredIndex_;
    mutable bool hoverOnMore_ = false;
    mutable bool hoverOnAdd_ = false;

    // Load once; let Qt's SVG icon engine render at the correct size/DPI.
    const QIcon moreIcon{":/images/Project/record-action-menu.svg"};
    const QIcon addIcon{":/images/Project/record-action-plus.svg"};

public:
    explicit ProjectTreeDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override {
        QStyleOptionViewItem opt(option);
        initStyleOption(&opt, index);

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        // Custom selection background
        if (opt.state & QStyle::State_Selected) {
            // Draw action button only for selected item
            if (index.data(Qt::UserRole+2).toString() == "Directory") {
                QRect btnPlusRect(opt.rect.right() - 28, opt.rect.top() + 3, 22, 22);
                QRect btnMenuRect(opt.rect.right() - 56, opt.rect.top() + 3, 22, 22);

                drawActionButton(painter, btnMenuRect, moreIcon, (index == hoveredIndex_ && hoverOnMore_));
                drawActionButton(painter, btnPlusRect, addIcon, (index == hoveredIndex_ && hoverOnAdd_));
            } else {
                QRect btnMenuRect(opt.rect.right() - 28, opt.rect.top() + 3, 22, 22);

                drawActionButton(painter, btnMenuRect, moreIcon, (index == hoveredIndex_ && hoverOnMore_));
            }
        }
        painter->restore();

        // Draw default text/icon
        QStyledItemDelegate::paint(painter, opt, index);
    }

    bool editorEvent(QEvent *event, QAbstractItemModel *, const QStyleOptionViewItem &option,
                     const QModelIndex &index) override {
        if (event->type() == QEvent::MouseMove) {
            if (index.data(Qt::UserRole+2).toString() == "Directory") {
                QRect btnPlusRect(option.rect.right() - 28, option.rect.top() + 3, 22, 22);
                QRect btnMenuRect(option.rect.right() - 56, option.rect.top() + 3, 22, 22);

                auto *mouseEvent = static_cast<QMouseEvent *>(event);
                hoveredIndex_ = index;
                hoverOnMore_ = btnMenuRect.contains(mouseEvent->pos());
                hoverOnAdd_ = btnPlusRect.contains(mouseEvent->pos());
            } else {
                QRect btnMenuRect(option.rect.right() - 28, option.rect.top() + 3, 22, 22);

                auto *mouseEvent = static_cast<QMouseEvent *>(event);
                hoveredIndex_ = index;
                hoverOnMore_ = btnMenuRect.contains(mouseEvent->pos());
            }

            // QAbstractItemView *view = qobject_cast<QAbstractItemView *>(option.widget);
            QAbstractItemView *view = const_cast<QAbstractItemView *>(qobject_cast<const QAbstractItemView *>(option.widget));
            if (view)
                view->viewport()->update();
        }

        return false;
    }
private:
    void drawActionButton(QPainter *painter, const QRect &rect, const QIcon &icon, bool hover) const {
        painter->save();
        QColor bgColor = hover ? QColor("#FFFFFF") : Qt::transparent;
        QColor borderColor = hover ? QColor("#1A1A1A") : Qt::transparent;

        // painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(borderColor);
        painter->setBrush(bgColor);
        painter->drawRoundedRect(rect, 5, 5);

        // Center the icon in the button (render SVG at correct DPI)
        const QSize iconSize(12, 12);
        const QPoint iconTopLeft(rect.center().x() - iconSize.width() / 2 - 2,
                                 rect.center().y() - iconSize.height() / 2 - 2);

        const qreal dpr = painter->device() ? painter->device()->devicePixelRatioF() : qreal(1.0);
        QPixmap pm = icon.pixmap(QSize(qRound(iconSize.width() * dpr), qRound(iconSize.height() * dpr)));
        pm.setDevicePixelRatio(dpr);
        painter->drawPixmap(iconTopLeft, pm);

        painter->restore();
    }
};

#endif // PROJECTTREEDELEGATE_H
