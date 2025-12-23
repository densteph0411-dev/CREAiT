#ifndef ADDITIONALFIELDLABEL_H
#define ADDITIONALFIELDLABEL_H

#include <QWidget>
#include "ui_additionalfieldlabel.h"

class AdditionalFieldLabel : public QWidget, public Ui::AdditionalFieldLabel
{
    Q_OBJECT

public:
    explicit AdditionalFieldLabel(QWidget *parent = nullptr);
    ~AdditionalFieldLabel();

    void setData(const QString& property, const QString& value);
};

#endif // ADDITIONALFIELDLABEL_H
