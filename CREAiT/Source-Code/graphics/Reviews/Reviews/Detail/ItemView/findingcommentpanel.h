#ifndef FINDINGCOMMENTPANEL_H
#define FINDINGCOMMENTPANEL_H

#include <QVBoxLayout>
#include <QWidget>
#include "ui_findingcommentpanel.h"
#include <reviewscontroller.h>

class FindingCommentPanel : public QWidget, public Ui::FindingCommentPanel
{
    Q_OBJECT

public:
    explicit FindingCommentPanel(QWidget *parent = nullptr);
    ~FindingCommentPanel();

    void initializeWidgets();
    void clearLayout();
    void clear(int type = FINDING);

    void setFindingData(const QString& parentId, const QString& itemId);
    void setCommentData(const QString& parentId, const QString& itemId, const QString& subItemId);

    void addFinding(const FindingData& data, bool adjustHeight = true);
    void addComment(const CommentData& data, bool adjustHeight = true);

    void adjustScrollHeight();

signals:
    void requestFinding(const QString& parentId, const QString& itemId, const QString& subItemId);

private slots:
    void clickedAddButton();

public:
    enum {
        FINDING,
        COMMENT
    };

private:
    QString reviewId;
    QString reviewItemId;
    QString findingId;
    int panelType;
    QVBoxLayout* containerLayout;
};

#endif // FINDINGCOMMENTPANEL_H
