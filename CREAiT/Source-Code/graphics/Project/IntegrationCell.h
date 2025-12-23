#ifndef INTEGRATIONCELL_H
#define INTEGRATIONCELL_H

#include <QWidget>

namespace Ui {
class IntegrationCell;
}

class IntegrationCell : public QWidget
{
    Q_OBJECT

public:
    explicit IntegrationCell(QWidget *parent = nullptr);
    ~IntegrationCell();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::IntegrationCell *ui;
};

#endif // INTEGRATIONCELL_H
