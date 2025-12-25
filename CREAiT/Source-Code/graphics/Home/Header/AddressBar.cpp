#include "addressbar.h"


AddressBar::AddressBar(QWidget *parent)
    : QWidget(parent), nextId(0), activeIndex(-1) {

    setupUi(this);

    mainLayout->setAlignment(Qt::AlignLeft);
}

AddressBar::~AddressBar() {

}

void AddressBar::addItem(const QIcon& icon, const QString& label, bool closable) {
    if (addressBarItems.size() > 0) {
        QLabel* separator = new QLabel("|");
        separator->setObjectName("addressBarSeparator");
        separator->setContentsMargins(10, 0, 10, 0);
        separator->setStyleSheet("color: grey;");

        mainLayout->addWidget(separator);
        separators.append(separator);
    }

    int existingId = findIdByLabel(label);

    qDebug() << "existingId" << existingId;

    if (existingId == -1) {
        int index = nextId++;
        auto* item = new AddressBarItem(index, icon, label, closable, this);
        mainLayout->addWidget(item);
        addressBarItems.insert(index, item);

        connect(item, &AddressBarItem::clicked, this, [this](int id, const QString& name) {
            setActiveIndex(id);
            emit activatedItem(id, name);
        });

        connect(item, &AddressBarItem::requestedClose, this, [this](int id, const QString& name) {
            emit closedItem(id, name);
            closeItem(id);
        });

        setActiveIndex(index);
    }
    else setActiveIndex(existingId);

}

void AddressBar::addStrech() {
    mainLayout->addStretch();
}

void AddressBar::setActiveIndex(int index) {
    if (index < 0)  return;

    for (auto it = addressBarItems.constBegin(); it != addressBarItems.constEnd(); ++it) {
        auto item = addressBarItems.value(it.key());
        item->setActive(false);
    }

    if (addressBarItems.contains(index)) {
        activeIndex = index;
        auto item = addressBarItems.value(index);
        item->setActive(true);
    }
}

void AddressBar::closeItem(int index) {
    if (addressBarItems.contains(index)) {
        AddressBarItem* item = addressBarItems.take(index);
        mainLayout->removeWidget(item);
        item->deleteLater();
    }

    if (!separators.isEmpty())
        delete separators.takeLast();

    if (!addressBarItems.isEmpty()) {
        auto it = std::prev(addressBarItems.constEnd());
        setActiveIndex(it.key());
    }
}

int AddressBar::findIdByLabel(const QString& label) const
{
    for (auto it = addressBarItems.constBegin(); it != addressBarItems.constEnd(); ++it) {
        AddressBarItem* item = it.value();

        if (!item) continue;

        if (item->label() == label) {
            qDebug() << "key" << it.key();
            return it.key();
        }
    }
    return -1;
}



