#ifndef AICONNECTIONSPANEL_H
#define AICONNECTIONSPANEL_H

#include <QMap>
#include <QVBoxLayout>
#include <QWidget>
#include "ui_aiconnectionspanel.h"
#include "aiconnectionpanel.h"
#include <records.h>

class AIConnectionsPanel : public QWidget, public Ui::AIConnectionsPanel
{
    Q_OBJECT

public:
    explicit AIConnectionsPanel(QWidget *parent = nullptr);
    ~AIConnectionsPanel();

    void initializeWidgets();
    void reset();
    void removeCreatedAIConnections();

signals:
    void requestBack();
    void requestNext();

private slots:
    void clickedAddButton();
    void handleDelete(int id);
    void clickedBackButton();
    void clickedNextButton();

private:
    int nextId;
    QMap<int, AIConnectionPanel*>* aiConnectionPanels;
    QString getProviderURL(const QString& aiTool);

    // IDs of AI connection records we created
    QStringList createdAIConnectionIDs;
};

#endif // AICONNECTIONSPANEL_H
