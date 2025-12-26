greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += core gui charts concurrent svg

CONFIG += c++20
# DEFINES += TEST_INTTEST

# === Include Paths ===
INCLUDEPATH += \
    $$PWD/Source-Code/3rdPartyLibraries/pugixml/src \
    $$PWD/Source-Code/dataObjects \
    $$PWD/Source-Code/fileOperations \
    $$PWD/Source-Code/code-tests \
    $$PWD/Source-Code/styleSheet \
    $$PWD/Source-Code/graphics/Test \
    $$PWD/Source-Code/code-tests \
    $$PWD/Source-Code/graphics \
    $$PWD/Source-Code/graphics/Common \
    $$PWD/Source-Code/graphics/Common/Dialogs \
    $$PWD/Source-Code/graphics/Common/Menus \
    $$PWD/Source-Code/graphics/Home \
    $$PWD/Source-Code/graphics/Home/SideBar \
    $$PWD/Source-Code/graphics/Home/Header \
    $$PWD/Source-Code/graphics/Project \
    $$PWD/Source-Code/graphics/Project/Dialogs \
    $$PWD/Source-Code/graphics/Project/Create \
    $$PWD/Source-Code/graphics/Project/View \
    $$PWD/Source-Code/graphics/Record \
    $$PWD/Source-Code/graphics/Record/Dialogs \
    $$PWD/Source-Code/graphics/Automations \
    $$PWD/Source-Code/graphics/Automations/Common \
    $$PWD/Source-Code/graphics/Automations/SideBar \
    $$PWD/Source-Code/graphics/Automations/Automation \
    $$PWD/Source-Code/graphics/Automations/Automation/View \
    $$PWD/Source-Code/graphics/Automations/Automation/Create \
    $$PWD/Source-Code/graphics/Automations/Automation/Detail \
    $$PWD/Source-Code/graphics/Automations/Automation/Test \
    $$PWD/Source-Code/graphics/Automations/Training \
    $$PWD/Source-Code/graphics/Automations/Training/View \
    $$PWD/Source-Code/graphics/Automations/Training/Create \
    $$PWD/Source-Code/graphics/Automations/Training/Detail \
    $$PWD/Source-Code/graphics/Documents \
    $$PWD/Source-Code/graphics/Reviews \
    $$PWD/Source-Code/graphics/Reviews/Common \
    $$PWD/Source-Code/graphics/Reviews/Reviews \
    $$PWD/Source-Code/graphics/Reviews/Reviews/View \
    $$PWD/Source-Code/graphics/Reviews/Reviews/Create \
    $$PWD/Source-Code/graphics/Reviews/Reviews/Detail \
    $$PWD/Source-Code/graphics/Reviews/Reviews/Detail/ItemView \
    $$PWD/Source-Code/graphics/Reviews/Standards \
    $$PWD/Source-Code/graphics/Reviews/Standards/View \
    $$PWD/Source-Code/graphics/Reviews/Standards/Create \
    $$PWD/Source-Code/graphics/Reviews/Checklists \
    $$PWD/Source-Code/graphics/Reviews/Checklists/View \
    $$PWD/Source-Code/graphics/Reviews/Checklists/Create \
    $$PWD/Source-Code/graphics/ProjectSettings \
    $$PWD/Source-Code/graphics/ProjectSettings/Common \
    $$PWD/Source-Code/graphics/ProjectSettings/BasicSettings \
    $$PWD/Source-Code/graphics/ProjectSettings/Integrations \
    $$PWD/Source-Code/graphics/ProjectSettings/AISettings \
    $$PWD/Source-Code/graphics/ProjectSettings/UserManagement \

# === Header Files ====
HEADERS += \
    Source-Code/3rdPartyLibraries/pugixml/scripts/pugixml_dll.rc \
    Source-Code/3rdPartyLibraries/pugixml/src/pugiconfig.hpp \
    Source-Code/3rdPartyLibraries/pugixml/src/pugixml.hpp \
    Source-Code/code-tests/IntegrationTestWorker.h \
    Source-Code/code-tests/integrationtest.h \
    Source-Code/dataObjects/DocumentRecord.h \
    Source-Code/dataObjects/records.h \
    Source-Code/dataObjects/dataDictionary.h \
    Source-Code/fileOperations/dpapi_crypto.h \
    Source-Code/fileOperations/generalFileOperations.h \
    Source-Code/fileOperations/reviewfileoperations.h \
    Source-Code/fileOperations/settings.h \
    Source-Code/graphics/Automations/Common/automationdatacontroller.h \
    Source-Code/graphics/Automations/Training/Create/dropfilewidget.h \
    Source-Code/graphics/Automations/Training/Detail/logitemwidget.h \
    Source-Code/graphics/Common/AppScale.h \
    Source-Code/graphics/Common/Menus/contextmenu.h \
    Source-Code/graphics/Common/appcontextcontroller.h \
    Source-Code/graphics/Common/contextsidebar.h \
    Source-Code/graphics/Common/flowlayout.h \
    Source-Code/graphics/Common/hseparatorwidget.h \
    Source-Code/graphics/Common/logmanager.h \
    Source-Code/graphics/Common/multiselectwidget.h \
    Source-Code/graphics/Common/passwordedit.h \
    Source-Code/graphics/Common/splashscreen.h \
    Source-Code/graphics/Common/Dialogs/deletedialog.h \
    Source-Code/graphics/Common/Dialogs/validationdialog.h \
    Source-Code/graphics/Common/Dialogs/adddocumentdialog.h \
    Source-Code/graphics/Common/Dialogs/spinnerwidget.h \
    Source-Code/graphics/Common/Dialogs/loadingdialog.h \
    Source-Code/graphics/Common/Dialogs/spinnerwidget.h \
    Source-Code/graphics/Common/richtexteditor.h \
    Source-Code/graphics/Common/windows_scale.h \
    Source-Code/graphics/Home/aichatbotpopup.h \
    Source-Code/graphics/Home/homepage.h \
    Source-Code/fileOperations/generalXMLOperations.h \
    Source-Code/fileOperations/projectFileOperations.h \
    Source-Code/fileOperations/recordFileOperations.h \
    Source-Code/graphics/Automations/Common/automationcell.h \
    Source-Code/graphics/Automations/Common/automationcellfooterpanel.h \
    Source-Code/graphics/Automations/Common/automationcellproperty.h \
    Source-Code/graphics/Automations/SideBar/automationssidebar.h \
    Source-Code/graphics/Automations/Automation/View/automationsviewpage.h \
    Source-Code/graphics/Automations/Automation/Detail/automationdetailpage.h \
    Source-Code/graphics/Automations/Automation/Create/automationcreatepage.h \
    Source-Code/graphics/Automations/Automation/Test/automationtestresultdialog.h \
    Source-Code/graphics/Automations/Training/Create/accordionwidget.h \
    Source-Code/graphics/Automations/Training/Create/trainingcreatepanel.h \
    Source-Code/graphics/Automations/Training/Detail/trainingdetailmainpanel.h \
    Source-Code/graphics/Automations/Training/Detail/trainingdetailpage.h \
    Source-Code/graphics/Automations/Training/Detail/trainingdetailtable.h \
    Source-Code/graphics/Automations/Training/Detail/trainingdetailtablecell.h \
    Source-Code/graphics/Automations/Training/Detail/trainingstatuspanel.h \
    Source-Code/graphics/Automations/Training/View/trainingsviewpage.h \
    Source-Code/graphics/Automations/Training/Create/trainingcreatepage.h \
    Source-Code/graphics/Automations/automationspage.h \
    Source-Code/graphics/Common/texteditor.h \
    Source-Code/graphics/Home/userinfodialog.h \
    Source-Code/graphics/Home/zoomsettingdialog.h \
    Source-Code/graphics/Project/Create/additionalfielditem.h \
    Source-Code/graphics/Project/Create/additionalfieldspanel.h \
    Source-Code/graphics/Project/Create/aiconnectionpanel.h \
    Source-Code/graphics/Project/Create/aiconnectionspanel.h \
    Source-Code/graphics/Project/Create/basicinfopanel.h \
    Source-Code/graphics/Project/Create/createprojectpage.h \
    Source-Code/graphics/Project/Create/createtypedialog.h \
    Source-Code/graphics/Project/Create/databaseconfigurationpanel.h \
    Source-Code/graphics/Project/Create/directoriespanel.h \
    Source-Code/graphics/Project/Create/directoryselectitem.h \
    Source-Code/graphics/Project/Create/integrationpanel.h \
    Source-Code/graphics/Project/Create/integrationspanel.h \
    Source-Code/graphics/Project/Create/stepprogressbar.h \
    Source-Code/graphics/Project/Dialogs/createdirectorydialog.h \
    Source-Code/graphics/Project/Dialogs/treecombobox.h \
    Source-Code/graphics/Project/IntegrationCell.h \
    Source-Code/graphics/Project/Presenter/projectdatapresenter.h \
    Source-Code/graphics/Project/ProjectInfoView.h \
    Source-Code/graphics/Project/ProjectTreeDelegate.h \
    Source-Code/graphics/Project/View/projectcard.h \
    Source-Code/graphics/Project/View/projectsviewpage.h \
    Source-Code/graphics/Project/aimodelcell.h \
    Source-Code/graphics/Project/projectpage.h \
    Source-Code/graphics/Project/projecttreemodel.h \
    Source-Code/graphics/Project/projecttreeview.h \
    Source-Code/graphics/Project/recordtreeitem.h \
    Source-Code/graphics/ProjectSettings/AISettings/aiconnection.h \
    Source-Code/graphics/ProjectSettings/AISettings/aiconnectionaddeditdialog.h \
    Source-Code/graphics/ProjectSettings/AISettings/aisettingspage.h \
    Source-Code/graphics/ProjectSettings/BasicSettings/additionalfieldtext.h \
    Source-Code/graphics/ProjectSettings/BasicSettings/basicsettingseditpage.h \
    Source-Code/graphics/ProjectSettings/BasicSettings/basicsettingspage.h \
    Source-Code/graphics/ProjectSettings/BasicSettings/customlineedit.h \
    Source-Code/graphics/ProjectSettings/BasicSettings/customtextedit.h \
    Source-Code/graphics/ProjectSettings/BasicSettings/dirselectedit.h \
    Source-Code/graphics/ProjectSettings/Common/accordion.h \
    Source-Code/graphics/ProjectSettings/Common/integrationinfopanel.h \
    Source-Code/graphics/ProjectSettings/Common/projectsettingscontroller.h \
    Source-Code/graphics/ProjectSettings/Integrations/integration.h \
    Source-Code/graphics/ProjectSettings/Integrations/integrationaddeditdialog.h \
    Source-Code/graphics/ProjectSettings/Integrations/integrationspage.h \
    Source-Code/graphics/ProjectSettings/UserManagement/useraddeditdialog.h \
    Source-Code/graphics/ProjectSettings/UserManagement/usermanagementpage.h \
    Source-Code/graphics/ProjectSettings/projectsettingspage.h \
    Source-Code/graphics/Record/Dialogs/linkrecordtablemodel.h \
    Source-Code/graphics/Record/Dialogs/recordaddlinkdialog.h \
    Source-Code/graphics/Record/Dialogs/recordsummarydialog.h \
    Source-Code/graphics/Record/Dialogs/recordsummarypopover.h \
    Source-Code/graphics/Record/Dialogs/versionhistorydialog.h \
    Source-Code/graphics/Record/Dialogs/versionitemwidget.h \
    Source-Code/graphics/Record/Presenter/recorddatapresenter.h \
    Source-Code/graphics/Record/createrecordform.h \
    Source-Code/graphics/Record/editrecordform.h \
    Source-Code/graphics/Record/recordview.h \
    Source-Code/graphics/Documents/documentcard.h \
    Source-Code/graphics/Documents/documentspage.h \
    Source-Code/graphics/Reviews/Checklists/Create/createchecklistpage.h \
    Source-Code/graphics/Reviews/Checklists/View/checklistitem.h \
    Source-Code/graphics/Reviews/Checklists/View/checklistsviewpage.h \
    Source-Code/graphics/Reviews/Common/reviewscontroller.h \
    Source-Code/graphics/Reviews/Reviews/Create/checklistdialog.h \
    Source-Code/graphics/Reviews/Reviews/Create/createreviewpage.h \
    Source-Code/graphics/Reviews/Reviews/Create/requestitem.h \
    Source-Code/graphics/Reviews/Reviews/Create/revieweritem.h \
    Source-Code/graphics/Reviews/Reviews/Create/reviewitem.h \
    Source-Code/graphics/Reviews/Reviews/Create/reviewitemcheckdialog.h \
    Source-Code/graphics/Reviews/Reviews/Create/reviewitemcheckwidget.h \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/additionalfieldlabel.h \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/findingcommentpanel.h \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/findingdialog.h \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/findingpage.h \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/reviewactiondialog.h \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/reviewitemdetailpage.h \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/reviewitemdetailtable.h \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/reviewitemdetailtableheader.h \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/reviewitemdetailtablerow.h \
    Source-Code/graphics/Reviews/Reviews/Detail/checklistitemwidget.h \
    Source-Code/graphics/Reviews/Reviews/Detail/checklistviewdialog.h \
    Source-Code/graphics/Reviews/Reviews/Detail/findingviewdialog.h \
    Source-Code/graphics/Reviews/Reviews/Detail/findingwidget.h \
    Source-Code/graphics/Reviews/Reviews/Detail/reviewdetailpage.h \
    Source-Code/graphics/Reviews/Reviews/Detail/revieweritemwidget.h \
    Source-Code/graphics/Reviews/Reviews/Detail/reviewitemcheckresultdialog.h \
    Source-Code/graphics/Reviews/Reviews/Detail/reviewitemcheckresultwidget.h \
    Source-Code/graphics/Reviews/Reviews/Detail/reviewitemwidget.h \
    Source-Code/graphics/Reviews/Reviews/Detail/reviewlistwidget.h \
    Source-Code/graphics/Reviews/Reviews/View/reviewsviewpage.h \
    Source-Code/graphics/Reviews/Standards/Create/standardcreatedialog.h \
    Source-Code/graphics/Reviews/Standards/View/standardcell.h \
    Source-Code/graphics/Reviews/Standards/View/standardsviewpage.h \
    Source-Code/graphics/Reviews/reviewspage.h \
    Source-Code/graphics/Test/testWindow.h \
    Source-Code/styleSheet/styleSheet.h \
    Source-Code/graphics/styleConstants.h \
    Source-Code/graphics/Home/mainWindow.h \
    Source-Code/graphics/Home/SideBar/sidebar.h \
    Source-Code/graphics/Home/Header/addressbaritem.h \
    Source-Code/graphics/Home/Header/addressbar.h \
    Source-Code/graphics/Home/Header/windowcontrolbar.h \
    Source-Code/graphics/Home/Header/header.h


# === Source Files ====
SOURCES += \
    Source-Code/3rdPartyLibraries/pugixml/src/pugixml.cpp \
    Source-Code/code-tests/integrationtest.cpp \
    Source-Code/dataObjects/DocumentRecord.cpp \
    Source-Code/dataObjects/records.cpp \
    Source-Code/dataObjects/dataDictionary.cpp \
    Source-Code/fileOperations/dpapi_crypto.cpp \
    Source-Code/fileOperations/generalFileOperations.cpp \
    Source-Code/fileOperations/reviewfileoperations.cpp \
    Source-Code/graphics/Automations/Common/automationdatacontroller.cpp \
    Source-Code/graphics/Automations/Training/Create/dropfilewidget.cpp \
    Source-Code/graphics/Automations/Training/Detail/logitemwidget.cpp \
    Source-Code/graphics/Common/Menus/contextmenu.cpp \
    Source-Code/graphics/Common/appcontextcontroller.cpp \
    Source-Code/graphics/Common/contextsidebar.cpp \
    Source-Code/graphics/Common/flowlayout.cpp \
    Source-Code/graphics/Common/hseparatorwidget.cpp \
    Source-Code/graphics/Common/logmanager.cpp \
    Source-Code/graphics/Common/multiselectwidget.cpp \
    Source-Code/graphics/Common/passwordedit.cpp \
    Source-Code/graphics/Common/splashscreen.cpp \
    Source-Code/graphics/Common/Dialogs/deletedialog.cpp \
    Source-Code/graphics/Common/Dialogs/validationdialog.cpp \
    Source-Code/graphics/Common/Dialogs/adddocumentdialog.cpp \
    Source-Code/graphics/Common/Dialogs/spinnerwidget.cpp \
    Source-Code/graphics/Common/Dialogs/loadingdialog.cpp \
    Source-Code/graphics/Common/richtexteditor.cpp \
    Source-Code/graphics/Home/aichatbotpopup.cpp \
    Source-Code/graphics/Home/homepage.cpp \
    Source-Code/fileOperations/generalXMLOperations.cpp \
    Source-Code/fileOperations/projectfileOperations.cpp \
    Source-Code/fileOperations/recordFileOperations.cpp \
    Source-Code/graphics/Automations/SideBar/automationssidebar.cpp \
    Source-Code/graphics/Automations/Common/automationcell.cpp \
    Source-Code/graphics/Automations/Common/automationcellproperty.cpp \
    Source-Code/graphics/Automations/Common/automationcellfooterpanel.cpp \
    Source-Code/graphics/Automations/Automation/View/automationsviewpage.cpp \
    Source-Code/graphics/Automations/Automation//Detail/automationdetailpage.cpp \
    Source-Code/graphics/Automations/Automation/Create/automationcreatepage.cpp \
    Source-Code/graphics/Automations/Automation/Test/automationtestresultdialog.cpp \
    Source-Code/graphics/Automations/Training/Create/accordionwidget.cpp \
    Source-Code/graphics/Automations/Training/Create/trainingcreatepanel.cpp \
    Source-Code/graphics/Automations/Training/Detail/trainingdetailmainpanel.cpp \
    Source-Code/graphics/Automations/Training/Detail/trainingdetailpage.cpp \
    Source-Code/graphics/Automations/Training/Detail/trainingdetailtable.cpp \
    Source-Code/graphics/Automations/Training/Detail/trainingdetailtablecell.cpp \
    Source-Code/graphics/Automations/Training/Detail/trainingstatuspanel.cpp \
    Source-Code/graphics/Automations/Training/View/trainingsviewpage.cpp \
    Source-Code/graphics/Automations/Training/Create/trainingcreatepage.cpp \
    Source-Code/graphics/Automations/automationspage.cpp \
    Source-Code/graphics/Common/texteditor.cpp \
    Source-Code/graphics/Home/userinfodialog.cpp \
    Source-Code/graphics/Home/zoomsettingdialog.cpp \
    Source-Code/graphics/Project/Create/additionalfielditem.cpp \
    Source-Code/graphics/Project/Create/additionalfieldspanel.cpp \
    Source-Code/graphics/Project/Create/aiconnectionpanel.cpp \
    Source-Code/graphics/Project/Create/aiconnectionspanel.cpp \
    Source-Code/graphics/Project/Create/basicinfopanel.cpp \
    Source-Code/graphics/Project/Create/createprojectpage.cpp \
    Source-Code/graphics/Project/Create/createtypedialog.cpp \
    Source-Code/graphics/Project/Create/databaseconfigurationpanel.cpp \
    Source-Code/graphics/Project/Create/directoriespanel.cpp \
    Source-Code/graphics/Project/Create/directoryselectitem.cpp \
    Source-Code/graphics/Project/Create/integrationpanel.cpp \
    Source-Code/graphics/Project/Create/integrationspanel.cpp \
    Source-Code/graphics/Project/Create/stepprogressbar.cpp \
    Source-Code/graphics/Project/Dialogs/createdirectorydialog.cpp \
    Source-Code/graphics/Project/Dialogs/treecombobox.cpp \
    Source-Code/graphics/Project/Presenter/projectdatapresenter.cpp \
    Source-Code/graphics/Project/View/projectcard.cpp \
    Source-Code/graphics/Project/View/projectsviewpage.cpp \
    Source-Code/graphics/Project/projecttreemodel.cpp \
    Source-Code/graphics/Project/projecttreeview.cpp \
    Source-Code/graphics/ProjectSettings/AISettings/aiconnection.cpp \
    Source-Code/graphics/ProjectSettings/AISettings/aiconnectionaddeditdialog.cpp \
    Source-Code/graphics/ProjectSettings/AISettings/aisettingspage.cpp \
    Source-Code/graphics/ProjectSettings/BasicSettings/additionalfieldtext.cpp \
    Source-Code/graphics/ProjectSettings/BasicSettings/basicsettingseditpage.cpp \
    Source-Code/graphics/ProjectSettings/BasicSettings/basicsettingspage.cpp \
    Source-Code/graphics/ProjectSettings/BasicSettings/customlineedit.cpp \
    Source-Code/graphics/ProjectSettings/BasicSettings/customtextedit.cpp \
    Source-Code/graphics/ProjectSettings/BasicSettings/dirselectedit.cpp \
    Source-Code/graphics/ProjectSettings/Common/accordion.cpp \
    Source-Code/graphics/ProjectSettings/Common/integrationinfopanel.cpp \
    Source-Code/graphics/ProjectSettings/Common/projectsettingscontroller.cpp \
    Source-Code/graphics/ProjectSettings/Integrations/integration.cpp \
    Source-Code/graphics/ProjectSettings/Integrations/integrationaddeditdialog.cpp \
    Source-Code/graphics/ProjectSettings/Integrations/integrationspage.cpp \
    Source-Code/graphics/ProjectSettings/UserManagement/useraddeditdialog.cpp \
    Source-Code/graphics/ProjectSettings/UserManagement/usermanagementpage.cpp \
    Source-Code/graphics/ProjectSettings/projectsettingspage.cpp \
    Source-Code/graphics/Record/Dialogs/linkrecordtablemodel.cpp \
    Source-Code/graphics/Record/Dialogs/recordaddlinkdialog.cpp \
    Source-Code/graphics/Record/Dialogs/recordsummarydialog.cpp \
    Source-Code/graphics/Record/Dialogs/recordsummarypopover.cpp \
    Source-Code/graphics/Record/Dialogs/versionhistorydialog.cpp \
    Source-Code/graphics/Record/Dialogs/versionitemwidget.cpp \
    Source-Code/graphics/Record/Presenter/recorddatapresenter.cpp \
    Source-Code/graphics/Record/createrecordform.cpp \
    Source-Code/graphics/Record/editrecordform.cpp \
    Source-Code/graphics/Record/recordview.cpp \
    Source-Code/graphics/Reviews/Checklists/Create/createchecklistpage.cpp \
    Source-Code/graphics/Reviews/Checklists/View/checklistitem.cpp \
    Source-Code/graphics/Reviews/Checklists/View/checklistsviewpage.cpp \
    Source-Code/graphics/Reviews/Common/reviewdataexample.cpp \
    Source-Code/graphics/Reviews/Common/reviewscontroller.cpp \
    Source-Code/graphics/Reviews/Reviews/Create/checklistdialog.cpp \
    Source-Code/graphics/Reviews/Reviews/Create/createreviewpage.cpp \
    Source-Code/graphics/Reviews/Reviews/Create/requestitem.cpp \
    Source-Code/graphics/Reviews/Reviews/Create/revieweritem.cpp \
    Source-Code/graphics/Reviews/Reviews/Create/reviewitem.cpp \
    Source-Code/graphics/Reviews/Reviews/Create/reviewitemcheckdialog.cpp \
    Source-Code/graphics/Reviews/Reviews/Create/reviewitemcheckwidget.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/additionalfieldlabel.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/findingcommentpanel.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/findingdialog.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/findingpage.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/reviewactiondialog.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/reviewitemdetailpage.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/reviewitemdetailtable.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/reviewitemdetailtableheader.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/reviewitemdetailtablerow.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/checklistitemwidget.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/checklistviewdialog.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/findingviewdialog.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/findingwidget.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/reviewdetailpage.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/revieweritemwidget.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/reviewitemcheckresultdialog.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/reviewitemcheckresultwidget.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/reviewitemwidget.cpp \
    Source-Code/graphics/Reviews/Reviews/Detail/reviewlistwidget.cpp \
    Source-Code/graphics/Reviews/Reviews/View/reviewsviewpage.cpp \
    Source-Code/graphics/Reviews/Standards/Create/standardcreatedialog.cpp \
    Source-Code/graphics/Reviews/Standards/View/standardcell.cpp \
    Source-Code/graphics/Reviews/Standards/View/standardsviewpage.cpp \
    Source-Code/graphics/Reviews/reviewspage.cpp \
    Source-Code/graphics/Test/testWindow.cpp \
    Source-Code/styleSheet/styleSheet.cpp \
    Source-Code/graphics/Home/mainWindow.cpp \
    Source-Code/graphics/Home/SideBar/sidebar.cpp \
    Source-Code/graphics/Home/Header/addressbaritem.cpp \
    Source-Code/graphics/Home/Header/addressbar.cpp \
    Source-Code/graphics/Home/Header/windowcontrolbar.cpp \
    Source-Code/graphics/Home/Header/header.cpp \
    Source-Code/graphics/Project/IntegrationCell.cpp \
    Source-Code/graphics/Project/ProjectInfoView.cpp \
    Source-Code/graphics/Project/aimodelcell.cpp \
    Source-Code/graphics/Project/projectpage.cpp \
    Source-Code/graphics/Project/recordtreeitem.cpp \
    Source-Code/graphics/Documents/documentcard.cpp \
    Source-Code/graphics/Documents/documentspage.cpp \
    Source-Code/main/main.cpp

# === Form Files ===
FORMS += \
    Source-Code/graphics/Automations/Training/Create/dropfilewidget.ui \
    Source-Code/graphics/Automations/Training/Detail/logitemwidget.ui \
    Source-Code/graphics/Common/Dialogs/deletedialog.ui \
    Source-Code/graphics/Common/Dialogs/validationdialog.ui \
    Source-Code/graphics/Common/Dialogs/adddocumentdialog.ui \
    Source-Code/graphics/Common/Dialogs/loadingdialog.ui \
    Source-Code/graphics/Common/splashscreen.ui \
    Source-Code/graphics/Automations/SideBar/automationssidebar.ui \
    Source-Code/graphics/Automations/Common/automationcell.ui \
    Source-Code/graphics/Automations/Common/automationcellfooterpanel.ui \
    Source-Code/graphics/Automations/Common/automationcellproperty.ui \
    Source-Code/graphics/Automations/Automation/View/automationsviewpage.ui \
    Source-Code/graphics/Automations/Automation/Create/automationcreatepage.ui \
    Source-Code/graphics/Automations/Automation/Detail/automationdetailpage.ui \
    Source-Code/graphics/Automations/Automation/Test/automationtestresultdialog.ui \
    Source-Code/graphics/Automations/Training/Create/accordionwidget.ui \
    Source-Code/graphics/Automations/Training/Create/trainingcreatepanel.ui \
    Source-Code/graphics/Automations/Training/Detail/trainingdetailmainpanel.ui \
    Source-Code/graphics/Automations/Training/Detail/trainingdetailpage.ui \
    Source-Code/graphics/Automations/Training/Detail/trainingdetailtable.ui \
    Source-Code/graphics/Automations/Training/Detail/trainingstatuspanel.ui \
    Source-Code/graphics/Automations/Training/View/trainingsviewpage.ui \
    Source-Code/graphics/Automations/Training/Create/trainingcreatepage.ui \
    Source-Code/graphics/Automations/automationspage.ui \
    Source-Code/graphics/Common/contextsidebar.ui \
    Source-Code/graphics/Common/hseparatorwidget.ui \
    Source-Code/graphics/Common/texteditor.ui \
    Source-Code/graphics/Home/aichatbotpopup.ui \
    Source-Code/graphics/Home/homepage.ui \
    Source-Code/graphics/Home/mainWindow.ui \
    Source-Code/graphics/Home/userinfodialog.ui \
    Source-Code/graphics/Home/zoomsettingdialog.ui \
    Source-Code/graphics/Project/Create/additionalfielditem.ui \
    Source-Code/graphics/Project/Create/additionalfieldspanel.ui \
    Source-Code/graphics/Project/Create/aiconnectionpanel.ui \
    Source-Code/graphics/Project/Create/aiconnectionspanel.ui \
    Source-Code/graphics/Project/Create/basicinfopanel.ui \
    Source-Code/graphics/Project/Create/createprojectpage.ui \
    Source-Code/graphics/Project/Create/createtypedialog.ui \
    Source-Code/graphics/Project/Create/databaseconfigurationpanel.ui \
    Source-Code/graphics/Project/Create/directoriespanel.ui \
    Source-Code/graphics/Project/Create/directoryselectitem.ui \
    Source-Code/graphics/Project/Create/integrationpanel.ui \
    Source-Code/graphics/Project/Create/integrationspanel.ui \
    Source-Code/graphics/Project/Create/stepprogressbar.ui \
    Source-Code/graphics/Project/Dialogs/createdirectorydialog.ui \
    Source-Code/graphics/Project/IntegrationCell.ui \
    Source-Code/graphics/Project/ProjectInfoView.ui \
    Source-Code/graphics/Project/View/projectcard.ui \
    Source-Code/graphics/Project/View/projectsviewpage.ui \
    Source-Code/graphics/Project/aimodelcell.ui \
    Source-Code/graphics/Project/projectpage.ui \
    Source-Code/graphics/ProjectSettings/AISettings/aiconnection.ui \
    Source-Code/graphics/ProjectSettings/AISettings/aiconnectionaddeditdialog.ui \
    Source-Code/graphics/ProjectSettings/AISettings/aisettingspage.ui \
    Source-Code/graphics/ProjectSettings/BasicSettings/additionalfieldtext.ui \
    Source-Code/graphics/ProjectSettings/BasicSettings/basicsettingseditpage.ui \
    Source-Code/graphics/ProjectSettings/BasicSettings/basicsettingspage.ui \
    Source-Code/graphics/ProjectSettings/BasicSettings/customlineedit.ui \
    Source-Code/graphics/ProjectSettings/BasicSettings/customtextedit.ui \
    Source-Code/graphics/ProjectSettings/BasicSettings/dirselectedit.ui \
    Source-Code/graphics/ProjectSettings/Common/accordion.ui \
    Source-Code/graphics/ProjectSettings/Common/integrationinfopanel.ui \
    Source-Code/graphics/ProjectSettings/Integrations/integration.ui \
    Source-Code/graphics/ProjectSettings/Integrations/integrationaddeditdialog.ui \
    Source-Code/graphics/ProjectSettings/Integrations/integrationspage.ui \
    Source-Code/graphics/ProjectSettings/UserManagement/useraddeditdialog.ui \
    Source-Code/graphics/ProjectSettings/UserManagement/usermanagementpage.ui \
    Source-Code/graphics/ProjectSettings/projectsettingspage.ui \
    Source-Code/graphics/Record/Dialogs/recordaddlinkdialog.ui \
    Source-Code/graphics/Record/Dialogs/recordsummarydialog.ui \
    Source-Code/graphics/Record/Dialogs/recordsummarypopover.ui \
    Source-Code/graphics/Record/Dialogs/versionhistorydialog.ui \
    Source-Code/graphics/Record/Dialogs/versionitemwidget.ui \
    Source-Code/graphics/Record/createrecordform.ui \
    Source-Code/graphics/Record/editrecordform.ui \
    Source-Code/graphics/Record/recordview.ui \
    Source-Code/graphics/Reviews/Checklists/Create/createchecklistpage.ui \
    Source-Code/graphics/Reviews/Checklists/View/checklistitem.ui \
    Source-Code/graphics/Reviews/Checklists/View/checklistsviewpage.ui \
    Source-Code/graphics/Reviews/Reviews/Create/checklistdialog.ui \
    Source-Code/graphics/Reviews/Reviews/Create/createreviewpage.ui \
    Source-Code/graphics/Reviews/Reviews/Create/requestitem.ui \
    Source-Code/graphics/Reviews/Reviews/Create/revieweritem.ui \
    Source-Code/graphics/Reviews/Reviews/Create/reviewitem.ui \
    Source-Code/graphics/Reviews/Reviews/Create/reviewitemcheckdialog.ui \
    Source-Code/graphics/Reviews/Reviews/Create/reviewitemcheckwidget.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/additionalfieldlabel.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/findingcommentpanel.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/findingdialog.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/findingpage.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/reviewactiondialog.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/reviewitemdetailpage.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/reviewitemdetailtable.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/reviewitemdetailtableheader.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/ItemView/reviewitemdetailtablerow.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/checklistitemwidget.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/checklistviewdialog.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/findingviewdialog.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/findingwidget.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/reviewdetailpage.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/revieweritemwidget.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/reviewitemcheckresultdialog.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/reviewitemcheckresultwidget.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/reviewitemwidget.ui \
    Source-Code/graphics/Reviews/Reviews/Detail/reviewlistwidget.ui \
    Source-Code/graphics/Reviews/Reviews/View/reviewsviewpage.ui \
    Source-Code/graphics/Reviews/Standards/Create/standardcreatedialog.ui \
    Source-Code/graphics/Reviews/Standards/View/standardcell.ui \
    Source-Code/graphics/Reviews/Standards/View/standardsviewpage.ui \
    Source-Code/graphics/Reviews/reviewspage.ui \
    Source-Code/graphics/Test/testWindow.ui \
    Source-Code/graphics/Home/SideBar/sidebar.ui \
    Source-Code/graphics/Home/Header/addressbaritem.ui \
    Source-Code/graphics/Home/Header/addressbar.ui \
    Source-Code/graphics/Home/Header/windowcontrolbar.ui \
    Source-Code/graphics/Home/Header/header.ui \
    Source-Code/graphics/Home/mainWindow.ui \
    Source-Code/graphics/Documents/documentcard.ui \
    Source-Code/graphics/Documents/documentspage.ui

LIBS += -lUser32  -lGdi32 -lShcore -ldwmapi

# === Translation Files ===
TRANSLATIONS += \
    Resources/translations/CREAiT_en_US.ts


# === Resource Files ===
RESOURCES += \
    Resources/Resources.qrc


# === Project Configuration ===
CONFIG += \
    lrelease \
    embed_translations


# === Customize High DPI ====
# RC_FILE = Resources/app.rc

# === Additional Files ===
DISTFILES += \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



