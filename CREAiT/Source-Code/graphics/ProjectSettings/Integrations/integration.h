#ifndef INTEGRATION_H
#define INTEGRATION_H

#include <QMouseEvent>
#include <QPaintEvent>
#include <QWidget>
#include "ui_integration.h"


class Integration : public QWidget, public Ui::Integration
{
    Q_OBJECT

public:
    explicit Integration(int pageId, int cellId, QWidget *parent = nullptr);
    ~Integration();

    int getCellId();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
    void requestRefresh(int pageId, int cellId);
    void requestEdit(int pageId, int cellId);
    void requestDelete(int pageId, int cellId);

private slots:
    void refresh();
    void showMenu();
    void edit();
    void remove();

private:
    int pageId;
    int cellId;
};

#endif // INTEGRATIONCELL_H
