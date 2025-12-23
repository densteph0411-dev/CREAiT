#include "checklistitemwidget.h"
#include "checklistviewdialog.h"

ChecklistItemWidget::ChecklistItemWidget(QWidget *parent)
    : QWidget(parent), index(-1)
{
    setupUi(this);
    connect(viewDetailButton, &QPushButton::clicked, this, &ChecklistItemWidget::clickedViewDetailButton);
}

ChecklistItemWidget::~ChecklistItemWidget() {

}

void ChecklistItemWidget::setData(int id, const ChecklistData& data) {
    index = id;
    checklistData = data;

    QString text = QString("%1. %2").arg(index).arg(data.title);
    mainLabel->setText(text);
}

void ChecklistItemWidget::clickedViewDetailButton() {
    ChecklistViewDialog* dialog = new ChecklistViewDialog(this);
    dialog->setData(checklistData.title, checklistData.questions);
    dialog->show();
}
