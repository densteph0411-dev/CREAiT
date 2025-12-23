#ifndef TRAININGSTATUSPANEL_H
#define TRAININGSTATUSPANEL_H

#include <QList>
#include <QVBoxLayout>
#include <QWidget>
#include "ui_trainingstatuspanel.h"
#include "logitemwidget.h"


class TrainingStatusPanel : public QWidget, public Ui::TrainingStatusPanel
{
    Q_OBJECT

public:
    explicit TrainingStatusPanel(QWidget *parent = nullptr);
    ~TrainingStatusPanel();

    void initializeWidgets();
    void appendLog(const QString& message, const QString& time);

    void appendExampleLogs();
protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void clickedLogDownloadButton();

private:
    QList<LogItemWidget*>* logItems;
    QVBoxLayout* logItemContainerLayout;
};

#endif // TRAININGSTATUSPANEL_H
