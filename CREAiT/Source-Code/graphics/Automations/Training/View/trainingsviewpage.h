#ifndef TRAININGSVIEWPAGE_H
#define TRAININGSVIEWPAGE_H

#include <QWidget>
#include <QMap>
#include <QGridLayout>
#include "ui_trainingsviewpage.h"
#include <automationcell.h>
#include <automationdatacontroller.h>


class TrainingsViewPage : public QWidget, public Ui::TrainingsViewPage
{
    Q_OBJECT

public:
    explicit TrainingsViewPage(QWidget *parent = nullptr);
    ~TrainingsViewPage();

    void initializeWidgets();
    void setTrainingWidgets();
    void refresh();

signals:
    void requestCreatePage(int pageId, const QString& cellId);
    void requestDetailPage(int pageId, const QString& cellId);
    void requestTrainingDelete(int pageId, const QString& cellId);

protected:
    void showEvent(QShowEvent* event) override;

public slots:
    void createTraining();
    void editTraining(int pageId, const QString& cellId);
    void deleteTraining(int pageId, const QString& cellId);

private:
    QMap<QString, AutomationData> trainings;
    QMap<QString, AutomationCell*> trainingWidgets;
    QGridLayout* containerLayout;
};

#endif // TRAININGSVIEWPAGE_H
