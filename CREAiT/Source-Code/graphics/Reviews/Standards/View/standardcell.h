#ifndef STANDARDCELL_H
#define STANDARDCELL_H

#include <QWidget>
#include "ui_standardcell.h"


class StandardCell : public QWidget, public Ui::StandardCell
{
    Q_OBJECT

public:
    explicit StandardCell(int id, QWidget *parent = nullptr);
    ~StandardCell();

    int cellId();
    void setData(const QString& title, const QString& filePath);

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;

signals:
    void requestEdit(int id);
    void requestRemove(StandardCell* cell);

private slots:
    void showMenu();
    void open();
    void edit();
    void remove();

private:
    int index;
    QString filePath;
};

#endif // STANDARDCELL_H
