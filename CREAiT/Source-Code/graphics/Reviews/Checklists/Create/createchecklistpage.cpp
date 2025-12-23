#include "createchecklistpage.h"
#include <reviewscontroller.h>

#include <QLayoutItem>

CreateChecklistPage::CreateChecklistPage(QWidget *parent)
    : QWidget(parent), index(-1), questionWidgetNextId(0)
{
    initializeWidgets();
}

CreateChecklistPage::~CreateChecklistPage() {

}

void CreateChecklistPage::initializeWidgets() {
    setupUi(this);

    containerLayout->setAlignment(Qt::AlignTop);

    questionItemsLayout = new QVBoxLayout(questionItems);
    questionItemsLayout->setContentsMargins(0, 0, 0, 0);
    questionItemsLayout->setSpacing(12);
    questionItemsLayout->setAlignment(Qt::AlignTop);

    connect(backButton, &QPushButton::clicked, this, &CreateChecklistPage::clickedBackButton);
    connect(cancelButton, &QPushButton::clicked, this, &CreateChecklistPage::clickedCancelButton);
    connect(saveButton, &QPushButton::clicked, this, &CreateChecklistPage::clickedSaveButton);
    connect(addQuestionButton, &QPushButton::clicked, this, &CreateChecklistPage::clickedAddQuestionButton);
}

void CreateChecklistPage::setData(int id) {
    index = id;

    QLayoutItem* item;
    while ((item = questionItemsLayout->takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        if (widget) {
            questionItemsLayout->removeWidget(widget);
            widget->deleteLater();
        }
        delete item;
    }

    questionWidgetNextId = 0;

    if (id == -1) {
        backButton->setText("Create Checklist");
        titleEdit->setText("");
        emit clickedAddQuestionButton();
    } else {
        const ChecklistData data = ReviewsController::getChecklistById(id);
        backButton->setText("Edit Checklist");
        titleEdit->setText(data.title);
        for (int i = 0; i < data.questions.size(); ++i) {
            int id = questionWidgetNextId++;
            AdditionalFieldItem* item = new AdditionalFieldItem(id);
            item->fieldNameEdit->setPlaceholderText("Enter question here");
            item->fieldNameEdit->setText(data.questions.at(i));

            questionWidgets.insert(id, item);
            questionItemsLayout->addWidget(item);

            connect(item, &AdditionalFieldItem::requestDelete, this, &CreateChecklistPage::handleDeleteQuestion);
        }
    }

    questionItems->adjustSize();
}

void CreateChecklistPage::clickedBackButton() {
    emit requestBackPage(2);
}

void CreateChecklistPage::clickedCancelButton() {
    emit clickedBackButton();
}

void CreateChecklistPage::clickedSaveButton() {
    ChecklistData data;
    data.title = titleEdit->text();

    for (int i = 0; i < questionItemsLayout->count(); ++i) {
        QWidget* widget = questionItemsLayout->itemAt(i)->widget();
        if (AdditionalFieldItem* item = qobject_cast<AdditionalFieldItem*>(widget)) {
            data.questions.append(item->fieldNameEdit->text());
        }
    }

    if (index == -1)
        ReviewsController::addChecklist(data);
    else
        ReviewsController::updateChecklist(index, data);

    emit clickedBackButton();
}

void CreateChecklistPage::clickedAddQuestionButton() {
    int id = questionWidgetNextId++;
    AdditionalFieldItem* item = new AdditionalFieldItem(id);
    item->fieldNameEdit->setPlaceholderText("Enter question here");

    questionWidgets.insert(id, item);
    questionItemsLayout->addWidget(item);

    questionItems->adjustSize();

    connect(item, &AdditionalFieldItem::requestDelete, this, &CreateChecklistPage::handleDeleteQuestion);
}

void CreateChecklistPage::handleDeleteQuestion(int id) {
    if (questionWidgets.size() <= 1) return;

    if (questionWidgets.contains(id)) {
        auto questionItem = questionWidgets.take(id);
        questionItemsLayout->removeWidget(questionItem);
        questionItem->deleteLater();
    }

    questionItems->adjustSize();
}
