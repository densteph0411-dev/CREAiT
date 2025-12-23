#ifndef REVIEWITEM_H
#define REVIEWITEM_H

#include <QWidget>
#include "ui_reviewitem.h"
#include <reviewscontroller.h>

class ReviewItem : public QWidget, public Ui::ReviewItem
{
    Q_OBJECT

public:
    explicit ReviewItem(int id, QWidget *parent = nullptr);
    ~ReviewItem();

    void initializeWidgets();
    ReviewItemData getData() const;
    void setData(const ReviewItemData& data);
    void refresh();

protected:
    void showEvent(QShowEvent* event) override;
signals:
    void requestDelete(int id);

private slots:
    void clickedDeleteButton();

private:
    int index;

};

#endif // REVIEWITEM_H
