#ifndef PROJECTSVIEWPAGE_H
#define PROJECTSVIEWPAGE_H

#include <QWidget>
#include "ui_projectsviewpage.h"
#include <deletedialog.h>
#include <flowlayout.h>
#include "projectcard.h"

class ProjectsViewPage : public QWidget, public Ui::ProjectsViewPage
{
    Q_OBJECT

public:
    explicit ProjectsViewPage(QWidget *parent = nullptr);
    ~ProjectsViewPage();

    void setProjectsPath(const QString& path);

    void refresh();

protected:
    void showEvent(QShowEvent* event);

signals:
    void requestCreateProject(int pageIndex);
    void requestViewProject(const QString& projectsPath, const QString& projectName);

private slots:
    void clickedLoadProjectButton();
    void clickedCreateProjectButton();
    void handleProjectView(const QString& projectName);
    void handleProjectCardDelete();

private:
    bool copyRecursively(const QString &srcPath, const QString &dstPath);

private:
    QString projectsPath;
    FlowLayout* layout;
    DeleteDialog* deleteDialog;
    ProjectCard* cardToDelete;
};

#endif // PROJECTSVIEWPAGE_H
