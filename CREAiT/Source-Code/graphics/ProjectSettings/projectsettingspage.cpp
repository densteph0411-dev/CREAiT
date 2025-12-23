#include "projectsettingspage.h"

ProjectSettingsPage::ProjectSettingsPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    connect(this, &ProjectSettingsPage::handleButtonSelect, contextSidebar, &ContextSidebar::handleButtonSelect);



    contextSidebar->setItemData(0, "Basic Settings",  ":/images/ProjectSettings/basic_settings.svg");
    contextSidebar->setItemData(1, "Integrations",  ":/images/ProjectSettings/integrations.svg");
    contextSidebar->setItemData(2, "AI Settings",  ":/images/ProjectSettings/ai_settings.svg");
    contextSidebar->setItemData(3, "User Management",  ":/images/ProjectSettings/user_management.svg");

    connect(contextSidebar, &ContextSidebar::requestPage, this, &ProjectSettingsPage::showPage);
    connect(basicSettingsPage, &BasicSettingsPage::requestEdit, this, &ProjectSettingsPage::showPage);
    connect(basicSettingsEditPage, &BasicSettingsEditPage::requestBackPage, this, &ProjectSettingsPage::showPage);
    connect(basicSettingsEditPage, &BasicSettingsEditPage::requestCancel, this, &ProjectSettingsPage::handleBasicSettingsEditCancel);
    connect(basicSettingsEditPage, &BasicSettingsEditPage::requestSave, this, &ProjectSettingsPage::handleBasicSettingsEditSave);

    navigateToPage(0);
}

ProjectSettingsPage::~ProjectSettingsPage()
{

}

void ProjectSettingsPage::navigateToPage(int pageIndex) {
    if (pageIndex < 0 || pageIndex > projectSettingsPageContainer->count() - 1) {
        qDebug() << "Invalid page index:" << pageIndex;
        return;
    }

    emit handleButtonSelect(pageIndex);
    projectSettingsPageContainer->setCurrentIndex(pageIndex);
}

void ProjectSettingsPage::showPage(int pageIndex) {
    if (pageIndex < 0 || pageIndex > projectSettingsPageContainer->count() - 1) {
        qDebug() << "Invalid page index:" << pageIndex;
        return;
    }
    projectSettingsPageContainer->setCurrentIndex(pageIndex);
}

void ProjectSettingsPage::handleBasicSettingsEditCancel() {

}

void ProjectSettingsPage::handleBasicSettingsEditSave() {

}

void ProjectSettingsPage::markDirty() {
    needsRefresh = true;
}

void ProjectSettingsPage::ensureRefreshed() {
    if (needsRefresh) {
        if (projectSettingsPageContainer->currentIndex() == 0)
            basicSettingsPage->loadWidgetsSettings();
        if (projectSettingsPageContainer->currentIndex() == 1)
            integrationsPage->refresh();
        if (projectSettingsPageContainer->currentIndex() == 2)
            aiSettingsPage->refresh();
        if (projectSettingsPageContainer->currentIndex() == 3)
            userManagementPage->refresh();
        needsRefresh = false;
    }
}
