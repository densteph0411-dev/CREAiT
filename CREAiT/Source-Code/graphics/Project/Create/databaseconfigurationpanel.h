#ifndef DATABASECONFIGURATIONPANEL_H
#define DATABASECONFIGURATIONPANEL_H

#include <QWidget>
#include "ui_databaseconfigurationpanel.h"


class DatabaseConfigurationPanel : public QWidget, public Ui::DatabaseConfigurationPanel
{
    Q_OBJECT

public:
    explicit DatabaseConfigurationPanel(QWidget *parent = nullptr);
    ~DatabaseConfigurationPanel();

    void initializeWidgets();
    void reset();
    
    QString getSelectedTool() const;

signals:
    void requestBack();
    void requestNext();


protected:
    void showEvent(QShowEvent *event) override;

private:
    void clickedBackButton();
    void clickedNextButton();
};

#endif // DATABASECONFIGURATIONPANEL_H
