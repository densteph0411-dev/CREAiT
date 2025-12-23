#ifndef CHECKLISTITEM_H
#define CHECKLISTITEM_H

#include <QLabel>
#include <QStringList>
#include <QVBoxLayout>
#include <QWidget>
#include "ui_checklistitem.h"


class ChecklistItem : public QWidget, public Ui::ChecklistItem
{
    Q_OBJECT

public:
    explicit ChecklistItem(int id, const QString& title, const QStringList& questions, QWidget *parent = nullptr);
    ~ChecklistItem();

    void initializeWidgets();
    void updateStyle();
    int itemId();
    void setData(const QString& title, const QStringList& questions);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
    void requestEdit(int id);
    void requestDelete(ChecklistItem* item);

private slots:
    void clickedMenuButton();
    void clickedToggleButton();
    void edit();
    void remove();

private:
    int index;
    QString title;
    QStringList questions;
    bool expanded;
    QVBoxLayout* bodyLayout;
};

#endif // CHECKLISTITEM_H
