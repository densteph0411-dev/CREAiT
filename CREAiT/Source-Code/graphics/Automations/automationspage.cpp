#include "automationspage.h"

AutomationsPage::AutomationsPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    connect(automationsSideBar, &AutomationsSideBar::requestedPage, this, &AutomationsPage::showPage);
    connect(this, &AutomationsPage::handleButtonSelect, automationsSideBar, &AutomationsSideBar::handleButtonSelect);

    connect(automationsViewPage, &AutomationsViewPage::requestCreatePage, this, &AutomationsPage::showPage);
    connect(automationsViewPage, &AutomationsViewPage::requestDetailPage, this, &AutomationsPage::showDetail);
    connect(automationDetailPage, &AutomationDetailPage::requestGoBackPage, this, &AutomationsPage::showPage);
    connect(automationDetailPage, &AutomationDetailPage::requestEdit, this, &AutomationsPage::showEdit);
    connect(automationCreatePage, &AutomationCreatePage::requestGoBackPage, this, &AutomationsPage::showPage);
    connect(automationCreatePage, &AutomationCreatePage::requestDetailPage, this, &AutomationsPage::showDetail);

    connect(trainingsViewPage, &TrainingsViewPage::requestCreatePage, this, &AutomationsPage::showPage);
    connect(trainingsViewPage, &TrainingsViewPage::requestDetailPage, this, &AutomationsPage::showDetail);
    connect(trainingDetailPage, &TrainingDetailPage::requestGoBack, this, &AutomationsPage::showPage);
    connect(trainingDetailPage, &TrainingDetailPage::requestEdit, this, &AutomationsPage::showEdit);
    connect(trainingCreatePage, &TrainingCreatePage::requestGoBackPage, this, &AutomationsPage::showPage);
    connect(trainingCreatePage, &TrainingCreatePage::requestDetailPage, this, &AutomationsPage::showDetail);

    this->setStyleSheet(loadStyleSheet(":/stylesheets/Automations/AutomationsPage.qss"));

    navigateToPage(0);
}

AutomationsPage::~AutomationsPage()
{

}

void AutomationsPage::navigateToPage(int pageIndex) {
    if (pageIndex < 0 || pageIndex > automationsPageContainer->count() - 1) {
        qDebug() << "Invalid page index:" << pageIndex;
        return;
    }

    emit handleButtonSelect(pageIndex);
    automationsPageContainer->setCurrentIndex(pageIndex);
}

void AutomationsPage::showPage(int pageIndex) {
    if (pageIndex < 0 || pageIndex > automationsPageContainer->count() - 1) {
        qDebug() << "Invalid page index:" << pageIndex;
        return;
    }

    if (pageIndex == AUTOMATIONS_DASHBOARD || pageIndex == TRAININGS_DASHBOARD)
        emit handleButtonSelect(pageIndex);

    switch (pageIndex) {
    case AUTOMATION_CREATE:
        automationCreatePage->setAutomationDataById();
        break;
    case TRAINING_CREATE:
        trainingCreatePage->setTrainingDetailById();
        break;
    }

    automationsPageContainer->setCurrentIndex(pageIndex);
}

void AutomationsPage::showDetail(int pageId, const QString& cellId) {
    if (pageId < 0 || pageId > automationsPageContainer->count() - 1) {
        qDebug() << "Invalid page index: " << pageId;
        return;
    }

    if (cellId.isEmpty()) {
        qDebug() << "Invalid cell index: " << cellId;
        return;
    }

    int navigatePageId = AUTOMATION_VIEW;
    switch(pageId) {
    case 0:
        navigatePageId = AUTOMATION_VIEW;
        automationDetailPage->setAutomationDetailById(cellId);
        break;
    default:
        navigatePageId = TRAINING_VIEW;
        trainingDetailPage->setTrainingDetailById(cellId);
        break;
    }

    automationsPageContainer->setCurrentIndex(navigatePageId);
}

void AutomationsPage::showEdit(int pageId, const QString& cellId) {
    if (pageId < 0 || pageId > automationsPageContainer->count() - 1) {
        qDebug() << "Invalid page index: " << pageId;
        return;
    }

    if (cellId.isEmpty()) {
        qDebug() << "Invalid cell index: " << cellId;
        return;
    }

    int navigatePageId = AUTOMATION_CREATE;
    switch(pageId) {
    case 0:
        navigatePageId = AUTOMATION_CREATE;
        automationCreatePage->setAutomationDataById(cellId);
        break;
    default:
        navigatePageId = TRAINING_CREATE;
        trainingCreatePage->setTrainingDetailById(cellId);
        break;
    }

    automationsPageContainer->setCurrentIndex(navigatePageId);
}
