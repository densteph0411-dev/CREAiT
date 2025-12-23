#ifndef REQUESTITEM_H
#define REQUESTITEM_H

#include "ui_requestitem.h"
#include <QWidget>
#include <reviewscontroller.h>

class RequestItem : public QWidget, public Ui::RequestItem
{
    Q_OBJECT

public:
    explicit RequestItem(int id, QWidget *parent = nullptr);
    ~RequestItem();

    RequestData getData() const;
    void setData(const RequestData& data);

signals:
    void requestDelete(int id);

private slots:
    void clickedDeleteButton();

private:
    int index;
};

#endif // REQUESTITEM_H
