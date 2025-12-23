#ifndef CHECKLISTITEMWIDGET_H
#define CHECKLISTITEMWIDGET_H

#include <QList>
#include <QWidget>

#include "ui_checklistitemwidget.h"
#include <reviewscontroller.h>

class ChecklistItemWidget : public QWidget, public Ui::ChecklistItemWidget
{
    Q_OBJECT

public:
    explicit ChecklistItemWidget(QWidget *parent = nullptr);
    ~ChecklistItemWidget();

    void setData(int id, const ChecklistData& data);

private slots:
    void clickedViewDetailButton();

private:
    int index;
    ChecklistData checklistData;
};

#endif // CHECKLISTITEMWIDGET_H
