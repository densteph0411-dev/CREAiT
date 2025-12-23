#ifndef CREATEREVIEWPAGE_H
#define CREATEREVIEWPAGE_H

#include <QMap>
#include <QVBoxLayout>
#include <QWidget>
#include "ui_createreviewpage.h"
#include "reviewitem.h"
#include "revieweritem.h"
#include "requestitem.h"


class CreateReviewPage : public QWidget, public Ui::CreateReviewPage
{
    Q_OBJECT

public:
    explicit CreateReviewPage(QWidget *parent = nullptr);
    ~CreateReviewPage();

    void initializeWidgets();
    void setData(const QString& id);

private:
    void removeAllReviewItems();
    void removeAllReviewerItems();
    void removeAllRequestItems();
    void clear();

signals:
    void requestBack(int pageIndex);

private slots:
    void clickedAddReviewItemButton();
    void clickedAddReviewerItemButton();
    void clickedAddRequestItemButton();
    void handleDeleteReviewItem(int id);
    void handleReviewItemCheck(int id);
    void handleDeleteReviewerItem(int id);
    void handleDeleteRequestItem(int id);

    void clickedAddChecklistButton();
    void clickedAddStandardButton();

    void clickedBackButton();
    void clickedCancelButton();
    void clickedSaveButton();

private:
    QString reviewId;
    int reviewItemNextId;
    int reviewerItemNextId;
    int requestItemNextId;
    QVBoxLayout* reviewItemLayout;
    QVBoxLayout* reviewerItemLayout;
    QVBoxLayout* requestItemLayout;
    QMap<int, ReviewItem*> reviewItems;
    QMap<int, ReviewerItem*> reviewerItems;
    QMap<int, RequestItem*> requestItems;
};

#endif // CREATEREVIEWPAGE_H
