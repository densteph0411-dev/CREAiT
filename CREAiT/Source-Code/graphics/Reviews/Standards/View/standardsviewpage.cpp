#include "standardsviewpage.h"
#include "qevent.h"
#include <QLayoutItem>
#include <QDir>
#include <standardcreatedialog.h>

StandardsViewPage::StandardsViewPage(QWidget *parent)
    : QWidget(parent), deleteCell(nullptr), reviewsPath("reviews")
{
    initializeWidgets();
}

StandardsViewPage::~StandardsViewPage()
{

}

void StandardsViewPage::initializeWidgets() {
    setupUi(this);

    QDir().mkdir(reviewsPath);

    layout = new FlowLayout(container, 0, 24);
    container->setLayout(layout);
    deleteDialog = new DeleteDialog();

    connect(uploadButton, &QPushButton::clicked, this, &StandardsViewPage::createStandard);
    connect(deleteDialog, &DeleteDialog::requestDelete, this, &StandardsViewPage::handleDelete);
}

void StandardsViewPage::refresh() {
    standards.clear();

    //  Remove all previously added widgets from layout
    for (int i = layout->count() - 1; i >= 0; --i) {
        QLayoutItem* item = layout->takeAt(i);
        if (item) {
            QWidget* widget = item->widget();
            if (widget) {
                widget->setParent(nullptr);  // detach from layout
                delete widget;               // safely delete widget
            }
            delete item; // delete layout item itself
        }
    }

    standards = ReviewsController::getStandards();

    for (auto it = standards.cbegin(); it != standards.cend(); ++it) {
        int id = it.key();
        const StandardData& data = it.value();
        StandardCell* cell = new StandardCell(id);
        cell->setData(data.title, data.filePath);

        connect(cell, &StandardCell::requestEdit, this, &StandardsViewPage::editStandard);
        connect(cell, &StandardCell::requestRemove, this, &StandardsViewPage::deleteStandard);

        layout->addWidget(cell);
    }
}

void StandardsViewPage::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    refresh();
}

void StandardsViewPage::createStandard() {
    StandardCreateDialog* createDialog = new StandardCreateDialog();
    createDialog->setData(-1);
    createDialog->show();

    connect(createDialog, &StandardCreateDialog::accepted, this, &StandardsViewPage::refresh);
}

void StandardsViewPage::editStandard(int id) {
    StandardCreateDialog* createDialog = new StandardCreateDialog();
    createDialog->setData(id);
    createDialog->show();

    connect(createDialog, &StandardCreateDialog::accepted, this, &StandardsViewPage::refresh);
}

void StandardsViewPage::deleteStandard(StandardCell* cell) {
    deleteCell = cell;
    deleteDialog->show();
}

void StandardsViewPage::handleDelete() {
    if (!deleteCell) return;

    int deleteId = deleteCell->cellId();
    layout->removeWidget(deleteCell);
    deleteCell->setParent(nullptr);
    delete deleteCell;

    ReviewsController::deleteStandardById(deleteId);
    deleteCell = nullptr;
}


