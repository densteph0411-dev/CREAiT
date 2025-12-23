#ifndef FINDINGWIDGET_H
#define FINDINGWIDGET_H

#include <QWidget>
#include "ui_findingwidget.h"
#include <reviewscontroller.h>


class FindingWidget : public QWidget, public Ui::FindingWidget
{
    Q_OBJECT

public:
    explicit FindingWidget(QWidget *parent = nullptr);
    ~FindingWidget();

    void initializeWidgets();
    void setFindingData(const QString& parentId, const QString& itemId, const FindingData& data);
    void setCommentData(const CommentData& data);

signals:
    void requestFinding(const QString& parentId, const QString& itemId, const QString& subItemId);

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    QString reviewId;
    QString reviewItemId;
    QString findingId;
    QString commentId;
};

#endif // FINDINGWIDGET_H
