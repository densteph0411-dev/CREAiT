#include "reviewitemdetailtablerow.h"
#include "additionalfieldlabel.h"
#include <QLayoutItem>

ReviewItemDetailTableRow::ReviewItemDetailTableRow(QWidget *parent)
    : QWidget(parent)
{
    initializeWidgets();
}

ReviewItemDetailTableRow::~ReviewItemDetailTableRow() {

}

void ReviewItemDetailTableRow::initializeWidgets() {
    setupUi(this);

    additionalFieldLayout = new QVBoxLayout(additionalFieldContainer);
    additionalFieldLayout->setContentsMargins(0, 0, 0, 0);
    additionalFieldLayout->setSpacing(12);
    additionalFieldLayout->setAlignment(Qt::AlignTop);

    linksModel = new QStandardItemModel(this);
    linksModel->setColumnCount(4);
    linksModel->setHorizontalHeaderLabels({"Link ID", "Title", "Relationship", "Type"});

    linksTable->setModel(linksModel);
    linksTable->setShowGrid(false);
    linksTable->setAlternatingRowColors(false);
    linksTable->setSelectionMode(QAbstractItemView::NoSelection);
    linksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    linksTable->setFocusPolicy(Qt::NoFocus);
    linksTable->setIconSize(QSize(20, 20));
    linksTable->horizontalHeader()->setFixedHeight(42);
    linksTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    linksTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    linksTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    linksTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    linksTable->horizontalHeader()->setStretchLastSection(true);
    linksTable->verticalHeader()->setVisible(false);
    linksTable->verticalHeader()->setDefaultSectionSize(42);
    // Resize columns to content
    linksTable->resizeColumnsToContents();
}

void ReviewItemDetailTableRow::setLastRow() {
    rowHeaderCell->setStyleSheet(R"(
        border-top-right-radius: 4px;
    )");

    linksCell->setStyleSheet(R"(
        border-bottom-right-radius: 4px;
    )");
}

void ReviewItemDetailTableRow::setData(const VersionData& data) {
    clear();

    rowHeaderLabel->setText("Version: " + data.id);
    titleLabel->setText(data.title);
    descriptionLabel->setText(data.description);
    statusLabel->setText(data.status);
    typeLabel->setText(data.type);

    AdditionalFieldLabel* recordLabel = new AdditionalFieldLabel(additionalFieldContainer);
    recordLabel->setData("Record File Path:", data.additionalField.recordFielPath);

    additionalFieldLayout->addWidget(recordLabel);
    for (auto itemData: data.additionalField.fieldItems) {
        AdditionalFieldLabel* fieldItem = new AdditionalFieldLabel(additionalFieldContainer);
        QString property = QString("%1:").arg(itemData.property);
        fieldItem->setData(property, itemData.value);

        additionalFieldLayout->addWidget(fieldItem);
    }

    for (const LinkData& link : data.links) {
        QList<QStandardItem*> row;
        row << new QStandardItem(link.linkId)
            << new QStandardItem(link.title)
            << new QStandardItem(link.relationship)
            << new QStandardItem(link.type);

        linksModel->appendRow(row);
    }
}

void ReviewItemDetailTableRow::clear() {
    rowHeaderLabel->setText("");
    titleLabel->setText("");
    descriptionLabel->setText("");
    statusLabel->setText("");
    typeLabel->setText("");

    QLayoutItem* item;
    while((item = additionalFieldLayout->takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        if (widget) {
            additionalFieldLayout->removeWidget(widget);
            widget->deleteLater();
        }
        delete item;
    }

    // additionalFieldContainer->adjustSize();
    // scrollArea->adjustSize();

    linksModel->removeRows(0, linksModel->rowCount());
}
