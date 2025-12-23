#ifndef INTEGRATIONINFOPANEL_H
#define INTEGRATIONINFOPANEL_H

#include <QWidget>
#include "ui_integrationinfopanel.h"

class IntegrationInfoPanel : public QWidget, public Ui::IntegrationInfoPanel
{
    Q_OBJECT

public:
    explicit IntegrationInfoPanel(QWidget *parent = nullptr);
    ~IntegrationInfoPanel();
};

#endif // INTEGRATIONINFOPANEL_H
