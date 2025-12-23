#ifndef REVIEWITEMDETAILTABLEROW_H
#define REVIEWITEMDETAILTABLEROW_H

#include <QWidget>
#include <QStandardItemModel>
#include "ui_reviewitemdetailtablerow.h"
#include <QVBoxLayout>
#include <reviewscontroller.h>

class ReviewItemDetailTableRow : public QWidget, public Ui::ReviewItemDetailTableRow
{
    Q_OBJECT

public:
    explicit ReviewItemDetailTableRow(QWidget *parent = nullptr);
    ~ReviewItemDetailTableRow();

    void initializeWidgets();

    void setLastRow();
    void setData(const VersionData& data);
    void clear();

private:
    int index;
    QVBoxLayout* additionalFieldLayout;
    QStandardItemModel *linksModel;
};

#endif // REVIEWITEMDETAILTABLEROW_H
