#ifndef BASICSETTINGSPAGE_H
#define BASICSETTINGSPAGE_H

#include <QShowEvent>
#include <QWidget>
#include "ui_basicsettingspage.h"
#include "automationcellproperty.h"
#include <projectsettingscontroller.h>

class BasicSettingsPage : public QWidget, public Ui::BasicSettingsPage
{
    Q_OBJECT

public:
    explicit BasicSettingsPage(QWidget *parent = nullptr);
    ~BasicSettingsPage();

    void initializeWidgets();
    void makeBasicInfoAccordion();
    void makeDirectoriesAccordion();
    void makeDatabaseAccordion();
    void makeAdditionalFieldsAccordion();
    void loadWidgetsSettings();

signals:
    void requestEdit(int index);

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    void clickedEditButton();

private:
    AutomationCellProperty* projectNameCell = nullptr;
    AutomationCellProperty* typeCell = nullptr;
    AutomationCellProperty* idPrefixCell = nullptr;
    AutomationCellProperty* credentialFileNameCell = nullptr;
    AutomationCellProperty* descriptionCell = nullptr;

    AutomationCellProperty* baseDirectoryCell = nullptr;
    AutomationCellProperty* programDirectoryCell = nullptr;
    AutomationCellProperty* projectDirectoryCell = nullptr;
    AutomationCellProperty* recordDirectoryCell = nullptr;
    AutomationCellProperty* imageDirectoryCell = nullptr;

    AutomationCellProperty* databaseNameCell = nullptr;
    AutomationCellProperty* databaseUrlCell = nullptr;
    AutomationCellProperty* cmUrlCell = nullptr;
    AutomationCellProperty* toolNameCell = nullptr;

    AutomationCellProperty* field1Cell = nullptr;
    AutomationCellProperty* field2Cell = nullptr;
};

#endif // BASICSETTINGSPAGE_H
