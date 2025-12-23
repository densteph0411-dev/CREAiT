#include "reviewitemcheckwidget.h"

ReviewItemCheckWidget::ReviewItemCheckWidget(QWidget *parent)
    : QWidget(parent)
{
    initializeWidgets();
}

ReviewItemCheckWidget::~ReviewItemCheckWidget() {

}

void ReviewItemCheckWidget::initializeWidgets() {
    setupUi(this);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(yesButton, 0);
    buttonGroup->addButton(noButton, 1);
    buttonGroup->addButton(notApplicableButton, 2);
}

void ReviewItemCheckWidget::setData(const QString& title, int checkedId, const QString& description) {
    QString formatedTitle = QString("%1. %2").arg(1).arg(title);

    questionLabel->setText(formatedTitle);
    buttonGroup->button(checkedId)->setChecked(true);
    descriptionEdit->setPlainText(description);
}
