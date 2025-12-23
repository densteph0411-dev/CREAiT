#ifndef DIRSELECTEDIT_H
#define DIRSELECTEDIT_H

#include <QWidget>
#include "ui_dirselectedit.h"

class DirSelectEdit : public QWidget, public Ui::DirSelectEdit
{
    Q_OBJECT

public:
    explicit DirSelectEdit(QWidget *parent = nullptr);
    ~DirSelectEdit();

public slots:
    void clickedDirSelectButton();
};

#endif // DIRSELECTEDIT_H
