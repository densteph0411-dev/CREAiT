#ifndef ADDRESSBAR_H
#define ADDRESSBAR_H

#include <QWidget>
#include <QIcon>
#include <QVector>
#include <QLabel>
#include <QMap>

#include "ui_addressbar.h"

#include <styleConstants.h>
#include <styleSheet.h>

#include "addressbaritem.h"

class AddressBar : public QWidget, public Ui::AddressBar, public StyleSheet
{
    Q_OBJECT

public:
    explicit AddressBar(QWidget *parent = nullptr);
    ~AddressBar();

    void addItem(const QIcon& icon, const QString& label, bool closable = true);
    void setActiveIndex(int index);
    void closeItem(int index);

    void addStrech();

    int findIdByLabel(const QString& label) const;

signals:
    void activatedItem(int index, const QString& name);
    void closedItem(int index, const QString& name);

private:
    int nextId;
    QMap<int, AddressBarItem*> addressBarItems;
    QVector<QLabel*> separators;
    int activeIndex;
};

#endif // ADDRESSBAR_H
