#ifndef INTEGRATIONSPAGE_H
#define INTEGRATIONSPAGE_H

#include <QGridLayout>
#include <QMap>
#include <QList>
#include <QWidget>
#include <QShowEvent>
#include "ui_integrationspage.h"

#include "integration.h"
#include "deletedialog.h"
#include <projectsettingscontroller.h>

class IntegrationsPage : public QWidget, public Ui::IntegrationsPage
{
    Q_OBJECT

public:
    explicit IntegrationsPage(QWidget *parent = nullptr);
    ~IntegrationsPage();

    Integration* createIntegrationCell(int id);
    void setIntegrations();
    void refresh();

public slots:
    void clickedCreateIntegrationButton();
    void refreshIntegration(int pageId, int cellId);
    void editIntegration(int pageId, int cellId);
    void deleteIntegration(int pageId, int cellId);
    void handleIntegrationDelete();

private:
    QMap<int, IntegrationData> integrationData;
    QList<Integration*>* integrations;
    QGridLayout* containerLayout;
    DeleteDialog* deleteDialog;
    int deleteId;
    bool dataInitialized;
    QString loadedProjectName;

protected:
    void showEvent(QShowEvent *event) override;
};

#endif // INTEGRATIONSPAGE_H
