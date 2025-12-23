#ifndef PROJECTSETTINGSPAGE_H
#define PROJECTSETTINGSPAGE_H

#include <QWidget>
#include "ui_projectsettingspage.h"

class ProjectSettingsPage : public QWidget, public Ui::ProjectSettingsPage
{
    Q_OBJECT

public:
    explicit ProjectSettingsPage(QWidget *parent = nullptr);
    ~ProjectSettingsPage();

    void navigateToPage(int pageIndex);
    void markDirty();
    void ensureRefreshed();

signals:
    void handleButtonSelect(int buttonIndex);

public slots:
    void showPage(int index);
    void handleBasicSettingsEditCancel();
    void handleBasicSettingsEditSave();

private:
    enum {
        BASIC_SETTINGS_PAGE,
        BASIC_SETTINGS_EDIT_PAGE,
    };
    bool needsRefresh;

};

#endif // PROJECTSETTINGSPAGE_H
