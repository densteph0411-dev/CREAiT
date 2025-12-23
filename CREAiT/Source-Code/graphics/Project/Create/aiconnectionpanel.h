#ifndef AICONNECTIONPANEL_H
#define AICONNECTIONPANEL_H

#include <QWidget>
#include "ui_aiconnectionpanel.h"

class AIConnectionPanel : public QWidget, public Ui::AIConnectionPanel
{
    Q_OBJECT

public:
    explicit AIConnectionPanel(int index = 0, QWidget *parent = nullptr);
    ~AIConnectionPanel();

signals:
    void requestDelete(int index);

private slots:
    void clickedDeleteButton();

private:
    int index;
};

#endif // AICONNECTIONPANEL_H
