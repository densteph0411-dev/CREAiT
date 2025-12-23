#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include <QMenu>
#include <QAction>
#include <QWidgetAction>
#include <QLabel>
#include <QString>

class ContextMenu : public QMenu
{
    Q_OBJECT
public:
    ContextMenu(bool edit = true);

signals:
    void requestEdit();
    void requestDelete();
};

#endif // CONTEXTMENU_H
