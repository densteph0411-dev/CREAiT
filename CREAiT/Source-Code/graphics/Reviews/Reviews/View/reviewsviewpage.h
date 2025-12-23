#ifndef REVIEWSVIEWPAGE_H
#define REVIEWSVIEWPAGE_H

#include <QStandardItemModel>
#include <QMap>
#include <QModelIndex>
#include <QWidget>
#include "ui_reviewsviewpage.h"
#include <deletedialog.h>
#include <reviewscontroller.h>


class ReviewsViewPage : public QWidget, public Ui::ReviewsViewPage
{
    Q_OBJECT

public:
    explicit ReviewsViewPage(QWidget *parent = nullptr);
    ~ReviewsViewPage();

    void initializeWidgets();
    void refresh();
    void setReviews();

protected:
    void showEvent(QShowEvent* event) override;

signals:
    void requestCreate(int pageIndex, const QString& reviewId);
    void requestDetail(int pageIndex, const QString& reviewId);

public slots:
    void clickedAddReviewButton();
    void doubleClickedTableItem(const QModelIndex& index);

    void handleViewClicked();
    void handleEditClicked();
    void handleDeleteClicked();
    void handleReviewDelete();

private:
    QStandardItemModel *reviewModel;
    DeleteDialog* deleteDialog;
    QMap<QString, ReviewData> reviewData;
    QString deleteId;
};

#endif // REVIEWSVIEWPAGE_H
