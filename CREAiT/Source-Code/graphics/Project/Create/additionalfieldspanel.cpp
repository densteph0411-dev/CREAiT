#include "additionalfieldspanel.h"
#include <records.h>

AdditionalFieldsPanel::AdditionalFieldsPanel(QWidget *parent)
    : QWidget(parent), nextId(0)
{
    setupUi(this);

    initializeWidgets();

    connect(addButton, &QPushButton::clicked, this, &AdditionalFieldsPanel::clickedAddButton);
    connect(backButton, &QPushButton::clicked, this, &AdditionalFieldsPanel::clickedBackButton);
    connect(nextButton, &QPushButton::clicked, this, &AdditionalFieldsPanel::clickedNextButton);
}

AdditionalFieldsPanel::~AdditionalFieldsPanel()
{

}

void AdditionalFieldsPanel::initializeWidgets() {
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    mainLayout->setAlignment(Qt::AlignTop);
    fieldLayout = new QVBoxLayout(itemContainer);
    fieldLayout->setContentsMargins(0, 0, 0, 0);
    fieldLayout->setSpacing(14);

    additionalFields = new QMap<int, AdditionalFieldItem*>();

    itemContainer->setFixedHeight(0);

    emit clickedAddButton();
}

void AdditionalFieldsPanel::clickedAddButton() {
    int id = nextId++;
    auto item = new AdditionalFieldItem(id, this);
    additionalFields->insert(id, item);
    fieldLayout->addWidget(item);
    connect(item, &AdditionalFieldItem::requestDelete, this, &AdditionalFieldsPanel::handleDelete);

    itemContainer->setFixedHeight(itemContainer->height() + 54);

    adjustSize();
}

void AdditionalFieldsPanel::handleDelete(int id) {
    if (additionalFields->size() <= 1) return;

    if (additionalFields->contains(id)) {
        AdditionalFieldItem *item = additionalFields->take(id);
        fieldLayout->removeWidget(item);
        item->deleteLater();

        itemContainer->setFixedHeight(itemContainer->height() - 54);
    }

    adjustSize();
}

void AdditionalFieldsPanel::clickedBackButton() {
    emit requestBack();
}

void AdditionalFieldsPanel::clickedNextButton()
{
    // Loop over all tiles/items and create fields in the Data dictionary
    QMapIterator<int, AdditionalFieldItem*> it(*additionalFields);
    while (it.hasNext()) {
        it.next();
        AdditionalFieldItem *item = it.value();
        if (!item) continue;

        // Get the field name from the UI
        const QString name = item->fieldNameEdit->text().trimmed();

        // Skip empty entries
        if (name.isEmpty())
            continue;

        const std::string fieldName = name.toStdString();

        // Add the field to the "record" context.
        //  fieldValue   -> "" (no default value for new records yet)
        //  persistence  -> "true" (store in project/record files)
        //  description  -> same as the user-visible name for now
        Data::addData(fieldName,
                      "record",
                      "",
                      "true",
                      fieldName);
    }

    // Move to the next step in the wizard
    emit requestNext();
}

void AdditionalFieldsPanel::reset() {
    QMapIterator<int, AdditionalFieldItem*> it(*additionalFields);
    while (it.hasNext()) {
        it.next();
        AdditionalFieldItem *item = it.value();
        fieldLayout->removeWidget(item);
        item->deleteLater();
    }
    additionalFields->clear();
    
    nextId = 0;
    itemContainer->setFixedHeight(0);
    clickedAddButton();
}

