#include "accordionwidget.h"

AccordionWidget::AccordionWidget(const int &id, const QString &title, QWidget *parent)
    : QWidget(parent), index(id), expanded(true)
{
    setupUi(this);

    headerLabel->setText(title);

    initializeWidgets();

    connect(toggleButton, &QPushButton::clicked, this, &AccordionWidget::clickedToggleButton);
    connect(deleteButton, &QPushButton::clicked, this, &AccordionWidget::clickedDeleteButton);

    connect(exampleInputSelect, &QComboBox::currentIndexChanged, this, &AccordionWidget::changedExampleInput);
    connect(exampleResultSelect, &QComboBox::currentIndexChanged, this, &AccordionWidget::changedExampleResult);

    updateStyle();
}

AccordionWidget::~AccordionWidget()
{

}

QStringList AccordionWidget::exampleItemList() const {
    return {
        "Test Case",
        "System Requirement",
        "Software Requirement",
        "Test Set",
        "Test Procedure",
        "Free Text/Code",
        "File Upload"
    };
}

void AccordionWidget::initializeWidgets() {
    const QStringList items = exampleItemList();
    exampleInputSelect->addItems(items);
    exampleResultSelect->addItems(items);

    exampleInputParamSelect = new QComboBox();
    textEditor = new TextEditor();
    exampleInputDropFileWidget = new DropFileWidget();

    exampleResultParamSelect = new QComboBox();
    exampleResultDropFileWidget = new DropFileWidget();

    exampleInputLayout->addWidget(exampleInputParamSelect);
    exampleInputLayout->addWidget(textEditor);
    exampleInputLayout->addWidget(exampleInputDropFileWidget);

    exampleResultLayout->addWidget(exampleResultParamSelect);
    exampleResultLayout->addWidget(exampleResultDropFileWidget);

    hideAllSubExampleInputWidgets();
    hideAllSubExampleResultWidgets();
}

void AccordionWidget::hideAllSubExampleInputWidgets() {
    exampleInputParamSelect->hide();
    textEditor->hide();
    exampleInputDropFileWidget->hide();
}

void AccordionWidget::hideAllSubExampleResultWidgets() {
    exampleResultParamSelect->hide();
    exampleResultDropFileWidget->hide();
}

int AccordionWidget::id() const {
    return index;
}

void AccordionWidget::updateStyle() {
    QString borderRadius = expanded ?
                               "border-top-left-radius: 8px; border-top-right-radius: 8px;" :
                               "border-radius: 8px;";

    QString headerStyle = QString(
                              "#header { background-color: white; border: 1px solid #34343426; %1 } "
                              ).arg(borderRadius);

    header->setStyleSheet(headerStyle);
}

void AccordionWidget::clickedToggleButton() {
    expanded = !expanded;
    contentArea->setVisible(expanded);
    toggleButton->setIcon(QIcon(expanded ? ":images/chevron-up.svg" : ":images/chevron-down.svg"));

    updateStyle();
}

void AccordionWidget::clickedDeleteButton() {
    emit requestDelelte(index);
}

void AccordionWidget::changedExampleInput(int index) {
    hideAllSubExampleInputWidgets();

    switch (index) {
    case TestCase:
    case SystemRequirement:
    case SoftwareRequirement:
    case TestSet:
    case TestProcedure:
        exampleInputParamSelect->show();
        break;
    case FreeText:
        textEditor->show();
        break;
    case FileUpload:
        exampleInputDropFileWidget->show();
        break;
    default:
        break;
    }
}

void AccordionWidget::changedExampleResult(int index) {
    hideAllSubExampleResultWidgets();
    switch (index) {
    case TestCase:
    case SystemRequirement:
    case SoftwareRequirement:
    case TestSet:
    case TestProcedure:
    case FreeText:
        exampleResultParamSelect->show();
        break;
    case FileUpload:
        exampleResultDropFileWidget->show();
        break;
    default:
        break;
    }
}

