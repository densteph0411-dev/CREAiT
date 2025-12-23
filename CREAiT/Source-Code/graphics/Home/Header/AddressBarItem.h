#ifndef ADDRESSBARITEM_H
#define ADDRESSBARITEM_H

#include <QWidget>
#include <QIcon>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>

#include "ui_addressbaritem.h"

#include <styleConstants.h>
#include <styleSheet.h>


class AddressBarItem : public QWidget, public Ui::AddressBarItem, public StyleSheet
{
    Q_OBJECT

public:
    explicit AddressBarItem(int id, const QIcon& icon, const QString& text, bool closable = true, QWidget *parent = nullptr);
    ~AddressBarItem();

    void setActive(bool active);
    bool active() const;

signals:
    void clicked(int index, const QString& name);
    void requestedClose(int index, const QString& name);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private slots:
    void clickedClose();

private:
    int index;
    QString name;
    bool isActive;
    bool showClose;
};

#endif // ADDRESSBARITEM_H
