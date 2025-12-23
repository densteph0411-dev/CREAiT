#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QScrollArea>
#include <QDir>
#include <QFileDialog>

#include <ui_homepage.h>

class HomePage : public QWidget, public Ui::HomePage
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

    bool isProjectExisted();
signals:
    void requestCreateProject(int pageIndex);
    void requestLoadProject(int pageIndex, const QString& projectsPath);

private slots:
    void clickedLoadProjectButton();
    void clickedCreateProjectButton();

private:
    QString projectsPath;
};

#endif // HOMEPAGE_H
