#ifndef AIMODELCELL_H
#define AIMODELCELL_H

#include <QWidget>
#include "projectsettingscontroller.h"

namespace Ui {
class AIModelCell;
}

class AIModelCell : public QWidget
{
    Q_OBJECT

public:
    explicit AIModelCell(QWidget *parent = nullptr);
    ~AIModelCell();

    void setData(AIConnectionData data);
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::AIModelCell *ui;
};

#endif // AIMODELCELL_H
