#ifndef ADDITIONALFIELDTEXT_H
#define ADDITIONALFIELDTEXT_H

#include <QWidget>
#include "ui_additionalfieldtext.h"


class AdditionalFieldText : public QWidget, public Ui::AdditionalFieldText
{
    Q_OBJECT

public:
    explicit AdditionalFieldText(int id, QWidget *parent = nullptr);
    ~AdditionalFieldText();

signals:
    void requestDelete(int id);

private slots:
    void clickedDeleteButton();

private:
    int index;
};

#endif // ADDITIONALFIELDTEXT_H
