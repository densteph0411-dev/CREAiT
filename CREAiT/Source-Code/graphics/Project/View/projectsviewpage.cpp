#include "projectsviewpage.h"
#include <records.h>
#include <settings.h>

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>

ProjectsViewPage::ProjectsViewPage(QWidget *parent)
    : QWidget(parent),
    deleteDialog(new DeleteDialog()) {
    setupUi(this);

    setProjectsPath(QString::fromStdString(Data::getDataValue("programDirectory", "project")));

    container->setLayout(new FlowLayout(container, 0, 17, 30));

    connect(createProjectButton, &QPushButton::clicked, this, &ProjectsViewPage::clickedCreateProjectButton);
    connect(loadProjectButton, &QPushButton::clicked, this, &ProjectsViewPage::clickedLoadProjectButton);
    connect(deleteDialog, &DeleteDialog::requestDelete, this, &ProjectsViewPage::handleProjectCardDelete);
}

ProjectsViewPage::~ProjectsViewPage() {

}

void ProjectsViewPage::setProjectsPath(const QString& path) {
    projectsPath = path;
}

void ProjectsViewPage::refresh() {
    if (projectsPath.isEmpty()) return;

    layout = reinterpret_cast<FlowLayout*>(container->layout());

    std::vector<std::string> projects = projectInfoRecord::getRecentProjects();

    for (int i = layout->count() - 1; i >= 0; --i) {
        QLayoutItem* item = layout->takeAt(i);
        if (item) {
            QWidget* widget = item->widget();
            if (widget) {
                widget->setParent(nullptr);
                delete widget;
            }
            delete item;
        }
    }
    for(auto project : projects) {
        ProjectCard* card = new ProjectCard(QString::fromStdString(project));
        layout->addWidget(card);

        connect(card, &ProjectCard::requestProjectView, this, &ProjectsViewPage::handleProjectView);
        connect(card, &ProjectCard::requestProjectDelete, this, [=](ProjectCard* projectCard) {
            cardToDelete = projectCard;
            deleteDialog->show();
        });
    }
}

void ProjectsViewPage::showEvent(QShowEvent* event) {
    refresh();

    QWidget::showEvent(event);
}

void ProjectsViewPage::clickedLoadProjectButton() {
    QString defaultPath = "../../Documentation/ExampleProject/testDirectory";
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select project.xml"), defaultPath,
                                                   tr("Project file (project.xml)"));
    if(filePath.isEmpty())
        return;

    // Set projectsPath to the directory containing the selected project.xml file
    QFileInfo fileInfo(filePath);
    setProjectsPath(fileInfo.absolutePath());
    Settings::save("projectsPath", projectsPath);
    Data::setDataValue("programDirectory", "project", projectsPath.toStdString());
    refresh();
}

void ProjectsViewPage::clickedCreateProjectButton() {
    emit requestCreateProject(2);
}


void ProjectsViewPage::handleProjectView(const QString& projectName) {
    emit requestViewProject(projectsPath, projectName);
}

void ProjectsViewPage::handleProjectCardDelete() {
    if (!cardToDelete) return;

    layout->removeWidget(cardToDelete);
    cardToDelete->setParent(nullptr);
    delete cardToDelete;

    cardToDelete = nullptr;
}

bool ProjectsViewPage::copyRecursively(const QString &srcPath, const QString &dstPath) {
    QDir sourceDir(srcPath);
    if(!sourceDir.exists())
        return false;

    QDir destDir(dstPath);
    if(!destDir.exists()) {
        if(!destDir.mkpath("."))
            return false;
    }

    const QFileInfoList entries = sourceDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    for(const QFileInfo &entry : entries) {
        const QString srcFilePath = entry.absoluteFilePath();
        const QString destFilePath = dstPath + "/" + entry.fileName();

        if(entry.isDir()) {
            if(!copyRecursively(srcFilePath, destFilePath))
                return false;
        } else {
            if(!QFile::copy(srcFilePath, destFilePath)) {
                QFile::remove(destFilePath);
                if(!QFile::copy(srcFilePath, destFilePath))
                    return false;
            }
        }
    }
    return true;
}
