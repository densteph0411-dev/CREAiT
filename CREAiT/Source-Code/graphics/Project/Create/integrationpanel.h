#ifndef INTEGRATIONPANEL_H
#define INTEGRATIONPANEL_H

#include <QWidget>
#include "ui_integrationpanel.h"

class IntegrationPanel : public QWidget, public Ui::IntegrationPanel
{
    Q_OBJECT

public:
    explicit IntegrationPanel(int index = 0, QWidget *parent = nullptr);
    ~IntegrationPanel();

signals:
    void requestDelete(int index);

private slots:
    void clickedDeleteButton();

private:
    int index;
};

#endif // INTEGRATIONPANEL_H
