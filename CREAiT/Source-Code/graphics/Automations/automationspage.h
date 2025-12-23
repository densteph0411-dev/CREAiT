#ifndef AUTOMATIONSPAGE_H
#define AUTOMATIONSPAGE_H

#include <QWidget>
#include <styleSheet.h>
#include "ui_automationspage.h"

class AutomationsPage : public QWidget, public StyleSheet, public Ui::AutomationsPage
{
    Q_OBJECT

public:
    explicit AutomationsPage(QWidget *parent = nullptr);
    ~AutomationsPage();

    void navigateToPage(int pageIndex);

signals:
    void handleButtonSelect(int buttonIndex);

public slots:
    void showPage(int pageIndex);
    void showDetail(int pageId, const QString& cellId);
    void showEdit(int pageId, const QString& cellId);

private:
    enum PageIndex {
        AUTOMATIONS_DASHBOARD,
        TRAININGS_DASHBOARD,
        AUTOMATION_CREATE,
        AUTOMATION_VIEW,
        TRAINING_CREATE,
        TRAINING_VIEW,
        END_PAGE
    };
};

#endif // AUTOMATIONSPAGE_H
