#include "reviewitemcheckresultwidget.h"

const QStringList answers = {"Yes", "No", "N/A"};

ReviewItemCheckResultWidget::ReviewItemCheckResultWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
}

ReviewItemCheckResultWidget::~ReviewItemCheckResultWidget() {

}

void ReviewItemCheckResultWidget::setData(const QString& question, int answerId, const QString& description) {
    questionLabel->setText(question);
    answerLabel->setText(answers[answerId]);
    descriptionLabel->setText(description);
}
