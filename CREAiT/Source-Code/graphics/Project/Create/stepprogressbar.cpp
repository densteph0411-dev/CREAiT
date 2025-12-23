#include "stepprogressbar.h"

StepProgressBar::StepProgressBar(QWidget *parent)
    : QWidget(parent), currentStep(0)
{
    setupUi(this);

    initializeWidgtets();
}

StepProgressBar::~StepProgressBar()
{

}

void StepProgressBar::initializeWidgtets() {
    upcomingIconPaths = new QStringList();
    upcomingIconPaths->append(":images/Project/step1_ready.svg");
    upcomingIconPaths->append(":images/Project/step2_upcoming.svg");
    upcomingIconPaths->append(":images/Project/step3_upcoming.svg");
    upcomingIconPaths->append(":images/Project/step4_upcoming.svg");
    upcomingIconPaths->append(":images/Project/step5_upcoming.svg");
    upcomingIconPaths->append(":images/Project/step6_upcoming.svg");

    readyIconPaths = new QStringList();
    readyIconPaths->append(":images/Project/step1_ready.svg");
    readyIconPaths->append(":images/Project/step2_ready.svg");
    readyIconPaths->append(":images/Project/step3_ready.svg");
    readyIconPaths->append(":images/Project/step4_ready.svg");
    readyIconPaths->append(":images/Project/step5_ready.svg");
    readyIconPaths->append(":images/Project/step6_ready.svg");

    lines = new QList<QPushButton*>();
    lines->append(line1);
    lines->append(line2);
    lines->append(line3);
    lines->append(line4);
    lines->append(line5);

    steps = new QList<QToolButton*>();
    steps->append(step1);
    steps->append(step2);
    steps->append(step3);
    steps->append(step4);
    steps->append(step5);
    steps->append(step6);

    labels = new QList<QLabel*>();
    labels->append(label1);
    labels->append(label2);
    labels->append(label3);
    labels->append(label4);
    labels->append(label5);
    labels->append(label6);
}

void StepProgressBar::setCurrentStep(int step) {
    currentStep = step;
}

void StepProgressBar::goBackStep() {
    if (currentStep <= 0) return;

    steps->at(currentStep)->setIcon(QIcon(upcomingIconPaths->at(currentStep)));
    labels->at(currentStep)->setStyleSheet("font-weight: 400;");

    currentStep--;

    steps->at(currentStep)->setIcon(QIcon(readyIconPaths->at(currentStep)));
    lines->at(currentStep)->setStyleSheet("background: #F0F0F0;");
}

void StepProgressBar::goNextStep() {
    if (currentStep >= steps->count() - 1) return;

    steps->at(currentStep)->setIcon(QIcon(":images/Project/step_completed.svg"));
    lines->at(currentStep)->setStyleSheet("background: #22084B;");
    labels->at(currentStep)->setStyleSheet("font-weight: 600;");

    currentStep++;

    steps->at(currentStep)->setIcon(QIcon(readyIconPaths->at(currentStep)));
}

void StepProgressBar::reset() {
    currentStep = 0;
    
    for (int i = 0; i < steps->count(); ++i) {
        if (i == 0) {
            steps->at(i)->setIcon(QIcon(readyIconPaths->at(i)));
            labels->at(i)->setStyleSheet("font-weight: 400;");
        } else {
            steps->at(i)->setIcon(QIcon(upcomingIconPaths->at(i)));
            labels->at(i)->setStyleSheet("font-weight: 400;");
        }
    }
    
    for (int i = 0; i < lines->count(); ++i) {
        lines->at(i)->setStyleSheet("background: #F0F0F0;");
    }
}