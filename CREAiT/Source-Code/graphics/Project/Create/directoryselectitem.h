#ifndef DIRECTORYSELECTITEM_H
#define DIRECTORYSELECTITEM_H

#include <QWidget>
#include "ui_directoryselectitem.h"

class DirectorySelectItem : public QWidget, public Ui::DirectorySelectItem
{
    Q_OBJECT

public:
    explicit DirectorySelectItem(QWidget *parent = nullptr);
    ~DirectorySelectItem();

    void reset();
    QString getPath() const;

private slots:
    void clickedDirectorySelectButton();
};

#endif // DIRECTORYSELECTITEM_H
