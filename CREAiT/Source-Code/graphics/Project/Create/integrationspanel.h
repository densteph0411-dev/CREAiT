#ifndef INTEGRATIONSPANEL_H
#define INTEGRATIONSPANEL_H

#include <QMap>
#include <QVBoxLayout>
#include <QWidget>
#include <QStringList>
#include "ui_integrationspanel.h"
#include "integrationpanel.h"

class IntegrationsPanel : public QWidget, public Ui::IntegrationsPanel
{
    Q_OBJECT

public:
    explicit IntegrationsPanel(QWidget *parent = nullptr);
    ~IntegrationsPanel();

    void removeCreatedIntegrations();
    void initializeWidgets();
    void reset();

signals:
    void requestBack();
    void requestNext();

private slots:
    void clickedBackButton();
    void clickedNextButton();
    void clickedAddButton();
    void handleDelete(int id);

private:
    int nextId;
    QMap<int, IntegrationPanel*>* integrationPanels;

    // IDs of connection records we created
    QStringList createdIntegrationIDs;
};

#endif // INTEGRATIONSPANEL_H
