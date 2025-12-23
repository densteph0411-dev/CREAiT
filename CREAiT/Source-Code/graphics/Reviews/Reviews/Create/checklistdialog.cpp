#include "checklistdialog.h"
#include <reviewscontroller.h>
#include <QPainter>

ChecklistDialog::ChecklistDialog(QWidget *parent)
    : QDialog(parent), questionWidgetNextId(0)
{
    initializeWidgets();
}

ChecklistDialog::~ChecklistDialog()
{

}

void ChecklistDialog::initializeWidgets() {
    setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    questionLayout = new QVBoxLayout(questionContainer);
    questionLayout->setContentsMargins(0, 0, 0, 0);
    questionLayout->setSpacing(12);
    questionLayout->setAlignment(Qt::AlignTop);

    connect(addQuestionButton, &QPushButton::clicked, this, &ChecklistDialog::clickedAddQuestionButton);
    connect(closeButton, &QPushButton::clicked, this, &ChecklistDialog::clickedCloseButton);
    connect(cancelButton, &QPushButton::clicked, this, &ChecklistDialog::clickedCancelButton);
    connect(saveButton, &QPushButton::clicked, this, &ChecklistDialog::clickedSaveButton);

    emit clickedAddQuestionButton();
}

void ChecklistDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void ChecklistDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}

void ChecklistDialog::clickedAddQuestionButton() {
    int id = questionWidgetNextId++;
    AdditionalFieldItem* item = new AdditionalFieldItem(id);
    item->fieldNameEdit->setPlaceholderText("Enter question here");

    questionWidgets.insert(id, item);
    questionLayout->addWidget(item);

    questionContainer->adjustSize();

    connect(item, &AdditionalFieldItem::requestDelete, this, &ChecklistDialog::handleDeleteQuestion);
}

void ChecklistDialog::handleDeleteQuestion(int id) {
    if (questionWidgets.size() <= 1) return;

    if (questionWidgets.contains(id)) {
        auto questionItem = questionWidgets.take(id);
        questionLayout->removeWidget(questionItem);
        questionItem->deleteLater();
    }

    questionContainer->adjustSize();
}

void ChecklistDialog::clickedCloseButton() {
    reject();
}

void ChecklistDialog::clickedCancelButton() {
    reject();
}

void ChecklistDialog::clickedSaveButton() {
    ChecklistData data;
    data.title = titleEdit->text();

    for (int i = 0; i < questionLayout->count(); ++i) {
        QWidget* widget = questionLayout->itemAt(i)->widget();
        if (AdditionalFieldItem* item = qobject_cast<AdditionalFieldItem*>(widget)) {
            data.questions.append(item->fieldNameEdit->text());
        }
    }

    ReviewsController::addChecklist(data);

    accept();
}
