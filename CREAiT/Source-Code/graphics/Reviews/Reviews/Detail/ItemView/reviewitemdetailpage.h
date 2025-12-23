#ifndef REVIEWITEMDETAILPAGE_H
#define REVIEWITEMDETAILPAGE_H

#include <QWidget>
#include "ui_reviewitemdetailpage.h"


class ReviewItemDetailPage : public QWidget, public Ui::ReviewItemDetailPage
{
    Q_OBJECT

public:
    explicit ReviewItemDetailPage(QWidget *parent = nullptr);
    ~ReviewItemDetailPage();

    void initializeWidgets();
    void setData(const QString& parentId, const QString& itemId);

signals:
    void requestBack(int pageId);
    void requestFinding(const QString& parentId, const QString& itemId, const QString& subItemId);

private:
    QString reviewId;
    QString reviewItemId;
};

#endif // REVIEWITEMDETAILPAGE_H
