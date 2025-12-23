#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QWidget>
#include "ui_customtextedit.h"

class CustomTextEdit : public QWidget, public Ui::CustomTextEdit
{
    Q_OBJECT

public:
    explicit CustomTextEdit(QWidget *parent = nullptr);
    ~CustomTextEdit();
};

#endif // CUSTOMTEXTEDIT_H
