#ifndef TRAININGDETAILTABLE_H
#define TRAININGDETAILTABLE_H

#include <QWidget>
#include "ui_trainingdetailtable.h"

class TrainingDetailTable : public QWidget, public Ui::TrainingDetailTable
{
    Q_OBJECT

public:
    explicit TrainingDetailTable(QWidget *parent = nullptr);
    ~TrainingDetailTable();
};

#endif // TRAININGDETAILTABLE_H
