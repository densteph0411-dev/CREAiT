#ifndef AISETTINGSPAGE_H
#define AISETTINGSPAGE_H

#include <QGridLayout>
#include <QList>
#include <QMap>
#include <QWidget>
#include <QShowEvent>
#include "ui_aisettingspage.h"

#include "aiconnection.h"
#include "deletedialog.h"
#include <projectsettingscontroller.h>

class AISettingsPage : public QWidget, public Ui::AISettingsPage
{
    Q_OBJECT

public:
    explicit AISettingsPage(QWidget *parent = nullptr);
    ~AISettingsPage();

    void setAIConnections();
    void refresh();

public slots:
    void clickedCreateAIConnectionButton();
    void refreshAIConnection(int pageId, int cellId);
    void editAIConnection(int pageId, int cellId);
    void deleteAIConnection(int pageId, int cellId);
    void handleAIConnectionDelete();

private:
    QMap<int, AIConnectionData> aiConnectionData;
    QList<AIConnection*>* aiConnections;
    QGridLayout* containerLayout;
    DeleteDialog* deleteDialog;
    int deleteId;
    bool dataInitialized;
    QString loadedProjectName;

protected:
    void showEvent(QShowEvent *event) override;
};

#endif // AISETTINGSPAGE_H
