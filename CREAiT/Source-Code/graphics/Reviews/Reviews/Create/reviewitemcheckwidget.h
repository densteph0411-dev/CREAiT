#ifndef REVIEWITEMCHECKWIDGET_H
#define REVIEWITEMCHECKWIDGET_H

#include <QButtonGroup>
#include <QWidget>
#include "ui_reviewitemcheckwidget.h"

class ReviewItemCheckWidget : public QWidget, public Ui::ReviewItemCheckWidget
{
    Q_OBJECT

public:
    explicit ReviewItemCheckWidget(QWidget *parent = nullptr);
    ~ReviewItemCheckWidget();

    void initializeWidgets();
    void setData(const QString& title, int checkedId, const QString& description);

private:
    QButtonGroup* buttonGroup;
};

#endif // REVIEWITEMCHECKWIDGET_H
