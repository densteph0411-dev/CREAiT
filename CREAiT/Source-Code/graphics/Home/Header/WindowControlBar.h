#ifndef WINDOWCONTROLBAR_H
#define WINDOWCONTROLBAR_H

#include <QWidget>

#include "ui_windowcontrolbar.h"

class WindowControlBar : public QWidget, public Ui::WindowControlBar
{
    Q_OBJECT

public:
    explicit WindowControlBar(QWidget *parent = nullptr);
    ~WindowControlBar();

signals:
    void handleAIChat();
    void handleZoom();
    void handleUser();

private slots:
    void clickedAIChat();
    void clickedZoom();
    void clickedUser();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // WINDOWCONTROLBAR_H
