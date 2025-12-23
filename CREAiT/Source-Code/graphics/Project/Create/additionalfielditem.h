#ifndef ADDITIONALFIELDITEM_H
#define ADDITIONALFIELDITEM_H

#include <QWidget>
#include "ui_additionalfielditem.h"

class AdditionalFieldItem : public QWidget, public Ui::AdditionalFieldItem
{
    Q_OBJECT

public:
    explicit AdditionalFieldItem(int id, QWidget *parent = nullptr);
    ~AdditionalFieldItem();

signals:
    void requestDelete(int id);

private slots:
    void clickedDeleteButton();

private:
    int index;
};

#endif // ADDITIONALFIELDITEM_H
