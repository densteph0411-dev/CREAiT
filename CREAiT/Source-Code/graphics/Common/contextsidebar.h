#ifndef CONTEXTSIDEBAR_H
#define CONTEXTSIDEBAR_H

#include <QWidget>
#include <QPushButton>
#include "ui_contextsidebar.h"

class ContextSidebar : public QWidget, public Ui::ContextSidebar
{
    Q_OBJECT

public:
    explicit ContextSidebar(QWidget *parent = nullptr);
    ~ContextSidebar();

    void setItemData(int id, const QString& title, const QString& iconPath, bool hide = false);
signals:
    void requestPage(int index);

public slots:
    void handleButtonSelect(int buttonIndex);

private:
    QList<QPushButton *>* pageButtons;
    QButtonGroup *buttonGroup;
};

#endif // CONTEXTSIDEBAR_H
