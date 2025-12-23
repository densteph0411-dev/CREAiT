#ifndef CREATECHECKLISTPAGE_H
#define CREATECHECKLISTPAGE_H

#include <QMap>
#include <QVBoxLayout>
#include <QWidget>
#include "ui_createchecklistpage.h"
#include <additionalfielditem.h>

class CreateChecklistPage : public QWidget, public Ui::CreateChecklistPage
{
    Q_OBJECT

public:
    explicit CreateChecklistPage(QWidget *parent = nullptr);
    ~CreateChecklistPage();

    void initializeWidgets();
    void setData(int id);

signals:
    void requestBackPage(int pageIndex);

private slots:
    void clickedBackButton();
    void clickedCancelButton();
    void clickedSaveButton();
    void clickedAddQuestionButton();
    void handleDeleteQuestion(int id);

private:
    int index;
    int questionWidgetNextId;
    QMap<int, AdditionalFieldItem*> questionWidgets;
    QVBoxLayout* questionItemsLayout;
};

#endif // CREATECHECKLISTPAGE_H
