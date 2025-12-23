#ifndef AUTOMATIONCELL_H
#define AUTOMATIONCELL_H

#include <QWidget>
#include <QPainter>
#include <ui_automationcell.h>

class AutomationCell : public QWidget, public Ui::AutomationCell
{
    Q_OBJECT

public:
    explicit AutomationCell(int pageId, const QString& cellId, QWidget *parent = nullptr);
    ~AutomationCell();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
    void requestEdit(int pageId, const QString& cellId);
    void requestDelete(int pageId, const QString& cellId);

private slots:
    void showMenu();
    void edit();
    void remove();

private:
    int pageId;
    QString cellId;
};

#endif // AUTOMATIONCELL_H
