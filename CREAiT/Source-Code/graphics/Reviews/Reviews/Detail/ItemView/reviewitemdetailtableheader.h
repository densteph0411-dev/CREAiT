#ifndef REVIEWITEMDETAILTABLEHEADER_H
#define REVIEWITEMDETAILTABLEHEADER_H

#include <QWidget>

namespace Ui {
class ReviewItemDetailTableHeader;
}

class ReviewItemDetailTableHeader : public QWidget
{
    Q_OBJECT

public:
    explicit ReviewItemDetailTableHeader(QWidget *parent = nullptr);
    ~ReviewItemDetailTableHeader();

private:
    Ui::ReviewItemDetailTableHeader *ui;
};

#endif // REVIEWITEMDETAILTABLEHEADER_H
