#ifndef CREATEPROJECTPAGE_H
#define CREATEPROJECTPAGE_H

#include <QWidget>
#include <QMessageBox>
#include "ui_createprojectpage.h"
#include "records.h"
#include "validationdialog.h"

class CreateProjectPage : public QWidget, public Ui::CreateProjectPage
{
    Q_OBJECT

public:
    explicit CreateProjectPage(QWidget *parent = nullptr);
    ~CreateProjectPage();

    void resetAllPanels();

    enum class Source { HomePage, ProjectsViewPage };
    void setSource(Source s) { source = s; }

signals:
    void requestBackPage(int pageId);
    void projectCreated(QString projectPath, QString projectName);

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void clickedCancelButton();
    void goBackPanel();
    void goNextPanel();
    void clickedFinishButton();

private:
    bool validateInputs();
    bool createProject();
    void updateRecordTypes(const QStringList& selectedRecordTypes);
    
    // NEW: ensure we only create the project once
    bool ensureProjectCreated();
    bool m_projectCreated = false;

    int currentIndex;
    Source source = Source::HomePage;
};

#endif // CREATEPROJECTPAGE_H
