#ifndef BASICSETTINGSEDITPAGE_H
#define BASICSETTINGSEDITPAGE_H

#include <QGridLayout>
#include <QMap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "ui_basicsettingseditpage.h"
#include "customlineedit.h"
#include "customtextedit.h"
#include "dirselectedit.h"
#include <projectsettingscontroller.h>
#include "additionalfieldtext.h"

class BasicSettingsEditPage : public QWidget, public Ui::BasicSettingsEditPage
{
    Q_OBJECT

public:
    explicit BasicSettingsEditPage(QWidget *parent = nullptr);
    ~BasicSettingsEditPage();

    void initializeWidgets();
    void makeBasicInfoAccordion();
    void makeDirectoriesAccordion();
    void makeDatabaseAccordion();
    void makeAdditionalFieldsAccordion();
    void loadWidgetsSettings();

signals:
    void requestBackPage(int index);
    void requestCancel(int index);
    void requestSave(int index);

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    void clickedBackButton();
    void clickedCancelButton();
    void clickedSaveButton();
    void clickedAddFieldButton();
    void handleDeleteField(int id);

private:
    CustomLineEdit* projectNameEdit;
    CustomLineEdit* typeEdit;
    CustomLineEdit* idPrefixEdit;
    CustomLineEdit* credentialsFileNameEdit;
    CustomTextEdit* descriptionEdit;

    DirSelectEdit* baseDirEdit;
    DirSelectEdit* programDirEdit;
    DirSelectEdit* projectDirEdit;
    DirSelectEdit* recordDirEdit;
    DirSelectEdit* imageDirEdit;

    CustomLineEdit* databaseNameEdit;
    CustomLineEdit* databaseUrlEdit;
    CustomLineEdit* cmUrlEdit;
    CustomLineEdit* toolNameEdit;

    int fieldNextId;
    QWidget* fieldTextContainer;
    QVBoxLayout* fieldTextContainerLayout;
    QMap<int, AdditionalFieldText*> fieldTexts;
    QPushButton* addFieldTextButton;
};

#endif // BASICSETTINGSEDITPAGE_H
