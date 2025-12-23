#ifndef REVIEWDETAILPAGE_H
#define REVIEWDETAILPAGE_H

#include <QWidget>
#include "ui_reviewdetailpage.h"

class ReviewDetailPage : public QWidget, public Ui::ReviewDetailPage
{
    Q_OBJECT

public:
    explicit ReviewDetailPage(QWidget *parent = nullptr);
    ~ReviewDetailPage();

    void initializeWidgets();
    void setData(const QString& id);

signals:
    void requestBack(int pageIndex);
    void requestEdit(int pageId, const QString& reviewId);
    void requestReviewItemDetail(const QString& reviewId, const QString& reviewItemDetailId);

private slots:
    void clickedBackButton();
    void clickedEditButton();
    void clickedSaveButton();

private:
    QString reviewId;
};

#endif // REVIEWDETAILPAGE_H
