#ifndef TRAININGCREATEPAGE_H
#define TRAININGCREATEPAGE_H

#include <QWidget>
#include "ui_trainingcreatepage.h"


class TrainingCreatePage : public QWidget, public Ui::TrainingCreatePage
{
    Q_OBJECT

public:
    explicit TrainingCreatePage(QWidget *parent = nullptr);
    ~TrainingCreatePage();

    void initStatusPanel();
    void setTrainingDetailById(const QString& id = "");

signals:
    void requestGoBackPage(int pageId);
    void requestDetailPage(int pageId, const QString& cellId);

private slots:
    void clickedBackButton();
    void clickedStartTrainingButton();
    void clickedSaveButton();

private:
    QString trainingId;
};

#endif // TRAININGCREATEPAGE_H
