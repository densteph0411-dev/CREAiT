#ifndef PROJECTCARD_H
#define PROJECTCARD_H

#include <QWidget>
#include <QFileInfo>

#include "ui_projectcard.h"

class ProjectCard : public QWidget, public Ui::ProjectCard {
    Q_OBJECT

public:
    explicit ProjectCard(const QString& name, QWidget *parent = nullptr);
    ~ProjectCard();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
    void requestProjectView(const QString& projectName);
    void requestProjectDelete(ProjectCard* card);

private slots:
    void showMenu();
    void edit();
    void remove();

private:
    QString projectName;
};

#endif // PROJECTCARD_H
