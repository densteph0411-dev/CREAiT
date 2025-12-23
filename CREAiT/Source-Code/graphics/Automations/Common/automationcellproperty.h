#ifndef AUTOMATIONCELLPROPERTY_H
#define AUTOMATIONCELLPROPERTY_H

#include <QWidget>
#include "ui_automationcellproperty.h"

class AutomationCellProperty : public QWidget, public Ui::AutomationCellProperty
{
    Q_OBJECT

public:
    explicit AutomationCellProperty(QWidget *parent = nullptr);
    ~AutomationCellProperty();
};

#endif // AUTOMATIONCELLPROPERTY_H
