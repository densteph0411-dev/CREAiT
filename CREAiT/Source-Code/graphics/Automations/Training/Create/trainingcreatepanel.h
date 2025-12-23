#ifndef TRAININGCREATEPANEL_H
#define TRAININGCREATEPANEL_H

#include <QWidget>
#include <QMap>
#include <QVBoxLayout>
#include "ui_trainingcreatepanel.h"

#include <accordionwidget.h>

class TrainingCreatePanel : public QWidget, public Ui::TrainingCreatePanel
{
    Q_OBJECT

public:
    explicit TrainingCreatePanel(QWidget *parent = nullptr);
    ~TrainingCreatePanel();

    void initializeWidgets();
private slots:
    void addDataset();
    void deleteDataset(int id);

private:
    QMap<int, AccordionWidget*> datasets;
    int nextId;
    QVBoxLayout* datasetLayout;

    QStringList aiModelSelectValues;
    QStringList versionSelectValues;
};

#endif // TRAININGCREATEPANEL_H
