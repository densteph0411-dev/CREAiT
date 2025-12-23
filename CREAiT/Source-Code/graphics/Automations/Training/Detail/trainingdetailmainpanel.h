#ifndef TRAININGDETAILMAINPANEL_H
#define TRAININGDETAILMAINPANEL_H

#include <QWidget>
#include "ui_trainingdetailmainpanel.h"

class TrainingDetailMainPanel : public QWidget, public Ui::TrainingDetailMainPanel
{
    Q_OBJECT

public:
    explicit TrainingDetailMainPanel(QWidget *parent = nullptr);
    ~TrainingDetailMainPanel();

    void setTrainingDetail(
        const QString &aiModelText,
        const QString &createdOnText,
        const QString &createdByText,
        const QString &prompt);
};

#endif // TRAININGDETAILMAINPANEL_H
