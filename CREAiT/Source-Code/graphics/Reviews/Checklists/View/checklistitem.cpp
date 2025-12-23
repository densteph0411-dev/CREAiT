#include "checklistitem.h"
#include "qevent.h"
#include <contextmenu.h>
#include <QWidgetAction>

ChecklistItem::ChecklistItem(int id, const QString& title, const QStringList& questions, QWidget *parent)
    : QWidget(parent), index(id), title(title), questions(questions), expanded(false)
{
    initializeWidgets();
}

ChecklistItem::~ChecklistItem() {

}

void ChecklistItem::initializeWidgets() {
    setupUi(this);

    updateStyle();
    titleLabel->setText(title);
    body->setVisible(expanded);

    connect(menuButton, &QPushButton::clicked, this, &ChecklistItem::clickedMenuButton);
    connect(toggleButton, &QPushButton::clicked, this, &ChecklistItem::clickedToggleButton);

    bodyLayout = new QVBoxLayout(body);
    bodyLayout->setContentsMargins(14, 0, 14, 12);
    bodyLayout->setSpacing(10);
    bodyLayout->setAlignment(Qt::AlignTop);

    for (int i = 0; i < questions.size(); ++i) {
        QString orderedQuestion = QString("%1. %2").arg(i + 1).arg(questions.at(i));
        QLabel* label = new QLabel(orderedQuestion);
        label->setFixedHeight(24);
        bodyLayout->addWidget(label);
    }

    body->adjustSize();
    // adjustSize();
}

void ChecklistItem::updateStyle() {
    QString borderRadius = expanded ?
                               "border-top-left-radius: 8px; border-top-right-radius: 8px; border-bottom: none;" :
                               "border-radius: 8px;";

    QString backgroundColor = expanded ? "#EEF4F6" : "white";

    QString headerStyle = QString(
                              "#header { background-color: %1; border: 1px solid #34343426; %2 } "
                              ).arg(backgroundColor, borderRadius);

    header->setStyleSheet(headerStyle);
}

int ChecklistItem::itemId() {
    return index;
}

void ChecklistItem::setData(const QString& title, const QStringList& questions) {
    titleLabel->setText(title);

    if (!bodyLayout) return;

    QLayoutItem* item;
    while ((item = bodyLayout->takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        if(widget) widget->deleteLater();
        delete item;
    }

    for (int i = 0; i < questions.size(); ++i) {
        QString orderedQuestion = QString("%1. %2").arg(i + 1).arg(questions.at(i));
        QLabel* label = new QLabel(orderedQuestion);
        label->setFixedHeight(24);
        bodyLayout->addWidget(label);
    }

    body->adjustSize();
}

void ChecklistItem::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit requestEdit(index);
    }
}


void ChecklistItem::clickedMenuButton() {
    ContextMenu* menu = new ContextMenu();

    connect(menu, &ContextMenu::requestEdit, this, &ChecklistItem::edit);
    connect(menu, &ContextMenu::requestDelete, this, &ChecklistItem::remove);

    QSize menuSize = menu->sizeHint();
    QPoint buttonPos = menuButton->mapToGlobal(QPoint(menuButton->width(), menuButton->height()));

    QPoint menuPos(buttonPos.x() - menuSize.width(), buttonPos.y());
    menu->exec(menuPos);
}

void ChecklistItem::clickedToggleButton() {
    expanded = !expanded;
    body->setVisible(expanded);
    toggleButton->setIcon(QIcon(expanded ? ":images/chevron-up.svg" : ":images/chevron-down.svg"));

    updateStyle();
}

void ChecklistItem::edit() {
    emit requestEdit(index);
}

void ChecklistItem::remove() {
    emit requestDelete(this);
}
