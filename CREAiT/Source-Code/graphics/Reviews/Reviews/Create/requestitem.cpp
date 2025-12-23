#include "requestitem.h"

RequestItem::RequestItem(int id, QWidget *parent)
    : QWidget(parent), index(id)
{
    setupUi(this);

    connect(deleteButton, &QPushButton::clicked, this, &RequestItem::clickedDeleteButton);
}

RequestItem::~RequestItem() {

}

RequestData RequestItem::getData() const {
    RequestData data;
    data.title = titleEdit->text();
    data.description = descriptionEdit->text();

    return data;
}

void RequestItem::setData(const RequestData& data) {
    titleEdit->setText(data.title);
    descriptionEdit->setText(data.description);
}

void RequestItem::clickedDeleteButton() {
    emit requestDelete(index);
}
