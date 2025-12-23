#ifndef ACCORDION_H
#define ACCORDION_H

#include <QWidget>
#include "ui_accordion.h"

class Accordion : public QWidget, public Ui::Accordion
{
    Q_OBJECT

public:
    explicit Accordion(QWidget *parent = nullptr);
    ~Accordion();

    void setTitle(const QString &title);
    void updateStyle();

public slots:
    void clickedToggleButton();

private:
    bool expanded;

};

#endif // ACCORDION_H
