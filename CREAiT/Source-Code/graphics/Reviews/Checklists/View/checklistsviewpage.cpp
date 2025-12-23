#include "checklistsviewpage.h"
#include <QLayoutItem>

#include "checklistitem.h"

ChecklistsViewPage::ChecklistsViewPage(QWidget *parent)
    : QWidget(parent)
{
    initializeWidgets();
}

ChecklistsViewPage::~ChecklistsViewPage()
{

}

void ChecklistsViewPage::initializeWidgets() {
    setupUi(this);

    containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 10, 0);
    containerLayout->setSpacing(16);
    containerLayout->setAlignment(Qt::AlignTop);

    deleteDialog = new DeleteDialog();
    connect(deleteDialog, &DeleteDialog::requestDelete, this, &ChecklistsViewPage::handleDelete);

    connect(createButton, &QPushButton::clicked, this, &ChecklistsViewPage::clickedCreateButton);
}

void ChecklistsViewPage::refresh() {
    QLayoutItem* item;
    while ((item = containerLayout->takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        if (widget) {
            containerLayout->removeWidget(widget);
            widget->deleteLater();
        }
        delete item;
    }

    checklists.clear();
    checklists = ReviewsController::getChecklists();

    for (auto it = checklists.cbegin(); it != checklists.cend(); ++it) {
        int id = it.key();
        const ChecklistData& data = it.value();
        ChecklistItem* item = new ChecklistItem(id, data.title, data.questions);

        connect(item, &ChecklistItem::requestEdit, this, &ChecklistsViewPage::editChecklist);
        connect(item, &ChecklistItem::requestDelete, this, &ChecklistsViewPage::deleteChecklist);

        containerLayout->addWidget(item);
    }
}

void ChecklistsViewPage::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    refresh();
}

void ChecklistsViewPage::clickedCreateButton() {
    emit requestCreatePage(3, -1);
}

void ChecklistsViewPage::editChecklist(int id) {
    emit requestCreatePage(3, id);
}


void ChecklistsViewPage::deleteChecklist(ChecklistItem* item) {
    deleteItem = item;
    deleteDialog->show();
}


void ChecklistsViewPage::handleDelete() {
    if (!deleteItem) return;

    int deleteId = deleteItem->itemId();
    containerLayout->removeWidget(deleteItem);
    deleteItem->setParent(nullptr);
    delete deleteItem;

    ReviewsController::deleteStandardById(deleteId);
    deleteItem = nullptr;
}
