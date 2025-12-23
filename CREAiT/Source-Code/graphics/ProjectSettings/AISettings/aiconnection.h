#ifndef AICONNECTION_H
#define AICONNECTION_H

#include <QWidget>
#include "ui_aiconnection.h"


class AIConnection : public QWidget, public Ui::AIConnection
{
    Q_OBJECT

public:
    explicit AIConnection(const int pageId, const int cellId, QWidget *parent = nullptr);
    ~AIConnection();

    int getCellId() { return cellId; }

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

#endif // AICONNECTION_H
