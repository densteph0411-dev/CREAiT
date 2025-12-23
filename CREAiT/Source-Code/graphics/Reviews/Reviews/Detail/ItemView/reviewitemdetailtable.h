#ifndef REVIEWITEMDETAILTABLE_H
#define REVIEWITEMDETAILTABLE_H

#include <QWidget>
#include "ui_reviewitemdetailtable.h"

class ReviewItemDetailTable : public QWidget, public Ui::ReviewItemDetailTable
{
    Q_OBJECT

public:
    explicit ReviewItemDetailTable(QWidget *parent = nullptr);
    ~ReviewItemDetailTable();

    void initializeWidget();
    void setData(const QString& parentId, const QString& reviewItemId);
};

#endif // REVIEWITEMDETAILTABLE_H
