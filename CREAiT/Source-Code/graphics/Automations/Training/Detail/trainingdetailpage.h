#ifndef TRAININGDETAILPAGE_H
#define TRAININGDETAILPAGE_H

#include <QWidget>
#include "ui_trainingdetailpage.h"

class TrainingDetailPage : public QWidget, public Ui::TrainingDetailPage
{
    Q_OBJECT

public:
    explicit TrainingDetailPage(QWidget *parent = nullptr);
    ~TrainingDetailPage();

    void setTrainingDetailById(const QString& id);
    
signals:
    void requestGoBack(int pageId);
    void requestEdit(int pageId, const QString& automationId);

public slots:
    void clickedBackButton();
    void clickedEditButton();

private:
    QString trainingId;
};

#endif // TRAININGDETAILPAGE_H
