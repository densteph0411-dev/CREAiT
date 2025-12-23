#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QWidget>
#include "ui_customlineedit.h"

class CustomLineEdit : public QWidget, public Ui::CustomLineEdit
{
    Q_OBJECT

public:
    explicit CustomLineEdit(QWidget *parent = nullptr);
    ~CustomLineEdit();
};

#endif // CUSTOMLINEEDIT_H
