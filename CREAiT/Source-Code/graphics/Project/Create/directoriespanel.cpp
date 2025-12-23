#include "basicinfopanel.h"
#include "directoriespanel.h"
#include "records.h"

void DirectoriesPanel::setBasicInfoPanel(BasicInfoPanel *panel)
{
    m_basicInfoPanel = panel;
}

DirectoriesPanel::DirectoriesPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    initializeWidgets();

    connect(backButton, &QPushButton::clicked, this, &DirectoriesPanel::clickedBackButton);
    connect(nextButton, &QPushButton::clicked, this, &DirectoriesPanel::clickedNextButton);
}

DirectoriesPanel::~DirectoriesPanel()
{

}

void DirectoriesPanel::initializeWidgets() {
    baseDirectory->label->setText("Base Directory");
    programDirectory->label->setText("Program Directory");
    projectDirectory->label->setText("Project Directory");
}

void DirectoriesPanel::clickedBackButton() {
    emit requestBack();
}

void DirectoriesPanel::clickedNextButton()
{
    const QString baseDir    = getBaseDirectory().trimmed();
    const QString programDir = getProgramDirectory().trimmed();
    const QString projectDir = getProjectDirectory().trimmed();

    emit requestNext();
}

void DirectoriesPanel::reset()
{
    // Prefer the live values from BasicInfoPanel (if available)
    QString baseDir;
    QString projectName;
    if (m_basicInfoPanel) {
        baseDir = m_basicInfoPanel->getProjectFilePath().trimmed();
        projectName = m_basicInfoPanel->getProjectName().trimmed();
    }

    // Reset + populate UI
    baseDirectory->reset();
    baseDirectory->directoryPathEdit->setText(baseDir);

    programDirectory->reset();
    const QString toolName = QString::fromStdString(Data::getDataValue("toolName", "project"));
    const QString programDir = (!baseDir.isEmpty() && !toolName.isEmpty())
                                   ? (baseDir + "/" + toolName)
                                   : QString();
    programDirectory->directoryPathEdit->setText(programDir);

    projectDirectory->reset();
    const QString projectDir = (!programDir.isEmpty() && !projectName.isEmpty())
                                   ? (programDir + "/" + projectName)
                                   : QString();
    projectDirectory->directoryPathEdit->setText(projectDir);
}

QString DirectoriesPanel::getBaseDirectory() const {
    return baseDirectory->getPath();
}

QString DirectoriesPanel::getProgramDirectory() const {



    return programDirectory->getPath();
}

QString DirectoriesPanel::getProjectDirectory() const {
    return projectDirectory->getPath();
}

void DirectoriesPanel::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    if (!m_basicInfoPanel)
        return;

    const QString baseDir =
        m_basicInfoPanel->getProjectFilePath().trimmed();
    const QString projectName =
        m_basicInfoPanel->getProjectName().trimmed();

    if (baseDir.isEmpty() || projectName.isEmpty())
        return;

    const QString programDir = baseDir + "/CREAiT";
    const QString projectDir = programDir + "/" + projectName;

    baseDirectory->directoryPathEdit->setText(baseDir);
    programDirectory->directoryPathEdit->setText(programDir);
    projectDirectory->directoryPathEdit->setText(projectDir);
}

