#include "homepage.h"
#include <QTimer>
#include <records.h>
#include <settings.h>

HomePage::HomePage(QWidget *parent) : QWidget(parent)
{
    setupUi(this);

    connect(loadProjectButton, &QPushButton::clicked, this, &HomePage::clickedLoadProjectButton);
    connect(createProjectButton, &QPushButton::clicked, this, &HomePage::clickedCreateProjectButton);

    // Load projectsPath from settings.json
    projectsPath = Settings::load("projectsPath", "../../Documentation/ExampleProject/testDirectory/CREAiT").toString();
    Data::setDataValue("programDirectory", "project", projectsPath.toStdString());

    QTimer::singleShot(0, this, [this]() {
        if (isProjectExisted())
            emit requestLoadProject(1, projectsPath);
    });
}

HomePage::~HomePage() {

}

bool HomePage::isProjectExisted() {
    QDir dir(projectsPath);
    QString filePath = dir.filePath("project.xml");
    if (QFile::exists(filePath))
        return true;
    else
        return false;
}


void HomePage::clickedLoadProjectButton() {
    QString defaultPath = "../../Documentation/ExampleProject/testDirectory";
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select project.xml"), defaultPath,
                                                   tr("Project file (project.xml)"));
    if(filePath.isEmpty())
        return;

    // Set projectsPath to the directory containing the selected project.xml file
    QFileInfo fileInfo(filePath);
    projectsPath = fileInfo.absolutePath();
    Settings::save("projectsPath", projectsPath);
    Data::setDataValue("programDirectory", "project", projectsPath.toStdString());
    emit requestLoadProject(1, projectsPath);
}

void HomePage::clickedCreateProjectButton() {
    emit requestCreateProject(2);
}
