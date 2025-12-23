#include "multiselectwidget.h"
#include <QDebug>
#include <QFile>
#include <QScrollBar>

MultiSelectWidget::MultiSelectWidget(QWidget *parent) : QWidget(parent),
    scrollArea(nullptr), scrollContent(nullptr)
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    QWidget* mainContainer = new QWidget(this);
    mainContainer->setObjectName("mainContainer");

    pillContainer = new QWidget(this);
    pillContainer->setObjectName("pillContainer");
    pillLayout = new FlowLayout(pillContainer, 0);
    pillContainer->setLayout(pillLayout);

    dropdownButton = new QToolButton(this);
    dropdownButton->setIcon(QIcon(":images/chevron-down.svg"));
    dropdownButton->setStyleSheet("QToolButton { border: none; padding: 5px 5px 0px 0px; }");
    dropdownButton->setCursor(Qt::PointingHandCursor);
    connect(dropdownButton, &QToolButton::clicked, this, &MultiSelectWidget::showPopup);

    QHBoxLayout *topLayout = new QHBoxLayout(mainContainer);
    topLayout->addWidget(pillContainer, 1);
    topLayout->addWidget(dropdownButton, 0);
    topLayout->setContentsMargins(5, 5, 5, 5);

    mainLayout->addWidget(mainContainer);

    createPopup();

    loadStyleSheet();
}

void MultiSelectWidget::addItem(const QString &item) {
    if (items.contains(item)) return;
    items.append(item);

    QHBoxLayout *itemLayout = new QHBoxLayout();
    itemLayout->setContentsMargins(0, 0, 10, 0);

    QLabel *label = new QLabel(shortText(item, 50));
    label->setProperty("class", "TagLabel");

    QCheckBox *check = new QCheckBox();
    check->setTristate(false);
    check->setCheckState(Qt::Unchecked);

    checkboxes[item] = check;
    connect(check, &QCheckBox::toggled, this, &MultiSelectWidget::onCheckboxToggled);

    itemLayout->addWidget(label);
    itemLayout->addStretch();
    itemLayout->addWidget(check);

    QVBoxLayout *scrollLayout = qobject_cast<QVBoxLayout *>(scrollContent->layout());
    if (scrollLayout) {
        // Ensure bottom margin is 0 to prevent gap
        scrollLayout->setContentsMargins(8, 8, 8, 0);
        scrollLayout->addLayout(itemLayout);
        updatePopupHeight();
    }
}

void MultiSelectWidget::addItems(const QStringList &newItems) {
    for (const QString &item : newItems) {
        addItem(item);
    }
}

void MultiSelectWidget::setCurrentItem(const QString& item) {
    if (checkboxes.contains(item)) {
        QCheckBox *check = checkboxes[item];
        if (!check->isChecked()) {
            check->setChecked(true);
        }
    }
}

void MultiSelectWidget::setCurrentItems(const QStringList &items) {
    for (const QString &item : items) {
        setCurrentItem(item);
    }
}


QStringList MultiSelectWidget::selectedItems() const {
    QStringList items;
    for (const QString& key: checkboxes.keys()) {
        if (checkboxes[key] && checkboxes[key]->isChecked())
            items.append(key);
    }

    return items;
}

void MultiSelectWidget::clear() {
    QLayout *scrollLayout = scrollContent->layout();
    if (scrollLayout) {
        QLayoutItem *child;
        while ((child = scrollLayout->takeAt(0)) != nullptr) {
            delete child;
        }
    }

    items.clear();
    checkboxes.clear();

    while (pillLayout->count() > 0) {
        QLayoutItem *item = pillLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    updatePopupHeight();
}

void MultiSelectWidget::clearSelections() {
    for (QCheckBox* checkbox : checkboxes) {
        if (checkbox && checkbox->isChecked()) {
            checkbox->setChecked(false);
        }
    }
    
    while (pillLayout->count() > 0) {
        QLayoutItem *item = pillLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
}

QString MultiSelectWidget::shortText(const QString& text, int length) {
    if (text.length() > length) {
        return text.left(length - 4) + "...";
    }
    return text;
}

void MultiSelectWidget::loadStyleSheet() {
    QFile file(":stylesheets/MultiSelectWidget.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QString::fromUtf8(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

void MultiSelectWidget::createPopup() {
    popup = new QFrame(this, Qt::Popup);
    popupLayout = new QVBoxLayout(popup);
    popupLayout->setContentsMargins(0, 0, 0, 0);
    popupLayout->setSpacing(0);

    scrollArea = new QScrollArea(popup);
    scrollContent = new QWidget();
    scrollContent->setObjectName("scrollContent");
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setContentsMargins(8, 8, 8, 0);
    scrollLayout->setSpacing(0);

    for (const QString &item : items) {
        QHBoxLayout *itemLayout = new QHBoxLayout();
        itemLayout->setContentsMargins(0, 0, 10, 0);
        QLabel *label = new QLabel(item);
        label->setProperty("class", "TagLabel");
        QCheckBox *check = new QCheckBox();
        checkboxes[item] = check;

        connect(check, &QCheckBox::toggled, this, &MultiSelectWidget::onCheckboxToggled);

        itemLayout->addWidget(label);
        itemLayout->addStretch();
        itemLayout->addWidget(check);
        scrollLayout->addLayout(itemLayout);
    }

    scrollContent->setLayout(scrollLayout);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    popupLayout->addWidget(scrollArea);

    updatePopupHeight();
}

void MultiSelectWidget::updatePopupHeight() {
    int visibleItemCount = 5;
    int itemHeight = 30;
    int totalItems = items.size();
    int popupHeight = qMin(totalItems, visibleItemCount) * itemHeight;

    scrollArea->setFixedHeight(popupHeight);
}

void MultiSelectWidget::showPopup() {
    QWidget* mainContainer = this->findChild<QWidget*>("mainContainer");
    if (mainContainer) {
        popup->setFixedWidth(mainContainer->width());
    }

    QPoint pos = mapToGlobal(mainContainer->geometry().bottomLeft());
    popup->move(pos);
    popup->show();
}

void MultiSelectWidget::onCheckboxToggled(bool checked) {
    QCheckBox *check = qobject_cast<QCheckBox *>(sender());
    QString text;
    for (const QString &key : checkboxes.keys()) {
        if (checkboxes[key] == check) {
            text = key;
            break;
        }
    }

    if (checked) addPill(text);
    else removePill(text);
}

void MultiSelectWidget::addPill(const QString &text) {
    for (int i = 0; i < pillLayout->count(); ++i) {
        QWidget *widget = pillLayout->itemAt(i)->widget();
        if (widget && widget->objectName() == text) return; // already added
    }

    QWidget *pill = new QWidget();
    pill->setObjectName(text);
    pill->setStyleSheet("QWidget { border: 1px solid #ccc; border-radius: 12px; padding: 2px 6px; background: transparent; }");
    QHBoxLayout *layout = new QHBoxLayout(pill);
    layout->setContentsMargins(6, 2, 6, 2);

    QLabel *label = new QLabel(shortText(text));
    label->setStyleSheet("QLabel { background: transparent; border: none; }");
    QPushButton *btn = new QPushButton("×");
    btn->setCursor(Qt::PointingHandCursor);
    btn->setStyleSheet("QPushButton { border: none; color: #444; } QPushButton:hover { color: red; }");
    connect(btn, &QPushButton::clicked, [=]() {
        checkboxes[text]->setChecked(false);
        removePill(text);
    });

    layout->addWidget(label);
    layout->addWidget(btn);
    pill->setLayout(layout);

    pillLayout->addWidget(pill);
}

void MultiSelectWidget::removePill(const QString &text) {
    for (int i = 0; i < pillLayout->count(); ++i) {
        QWidget *widget = pillLayout->itemAt(i)->widget();
        if (widget && widget->objectName() == text) {
            pillLayout->removeWidget(widget);
            widget->deleteLater();
            break;
        }
    }
}
