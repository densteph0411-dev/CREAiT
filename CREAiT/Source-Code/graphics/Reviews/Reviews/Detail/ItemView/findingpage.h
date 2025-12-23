#ifndef FINDINGPAGE_H
#define FINDINGPAGE_H

#include <QMenu>
#include <QWidget>
#include "ui_findingpage.h"


class FindingPage : public QWidget, public Ui::FindingPage
{
    Q_OBJECT

public:
    explicit FindingPage(QWidget *parent = nullptr);
    ~FindingPage();

    void initializeWidgets();
    void setData(const QString& parentId, const QString& itemId, const QString& subItemId);

signals:
    void requestBack(int pageId);

private slots:
    void clickedBackButton();
    void clickedMenuButton();
    void requestImplement();
    void requestVerification();
    void requestReject();

private:
    QString reviewId;
    QString reviewItemId;
    QString findingId;

    QMenu* actionMenu;
};

#endif // FINDINGPAGE_H
