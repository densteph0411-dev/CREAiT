#ifndef STEPPROGRESSBAR_H
#define STEPPROGRESSBAR_H

#include <QList>
#include <QWidget>
#include "ui_stepprogressbar.h"

class StepProgressBar : public QWidget, public Ui::StepProgressBar
{
    Q_OBJECT

public:
    explicit StepProgressBar(QWidget *parent = nullptr);
    ~StepProgressBar();

    void initializeWidgtets();

    void setCurrentStep(int step);
    void goBackStep();
    void goNextStep();
    void reset();
private:
    int currentStep;
    QStringList* upcomingIconPaths;
    QStringList* readyIconPaths;
    QList<QPushButton*>* lines;
    QList<QToolButton*>* steps;
    QList<QLabel*>* labels;

};

#endif // STEPPROGRESSBAR_H
