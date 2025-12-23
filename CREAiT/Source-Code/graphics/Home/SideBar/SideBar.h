#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QList>
#include <QToolButton>

#include "ui_sidebar.h"

#include <styleConstants.h>
#include <styleSheet.h>

class SideBar : public QWidget, public Ui::SideBar, public StyleSheet {
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);
    ~SideBar();

signals:
    void requestedPage(int pageIndex);

public slots:
    void selectedButton(int buttonIndex);

private:
    QList<QToolButton*>* pageButtons;
};

#endif // SIDEBAR_H
