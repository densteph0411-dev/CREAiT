#ifndef CHECKLISTDIALOG_H
#define CHECKLISTDIALOG_H

#include <QDialog>
#include <QMap>
#include <QVBoxLayout>
#include "ui_checklistdialog.h"
#include <additionalfielditem.h>


class ChecklistDialog : public QDialog, public Ui::ChecklistDialog
{
    Q_OBJECT

public:
    explicit ChecklistDialog(QWidget *parent = nullptr);
    ~ChecklistDialog();

    void initializeWidgets();

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void clickedAddQuestionButton();
    void handleDeleteQuestion(int id);

    void clickedCloseButton();
    void clickedCancelButton();
    void clickedSaveButton();
private:
    int questionWidgetNextId;
    QMap<int, AdditionalFieldItem*> questionWidgets;
    QVBoxLayout* questionLayout;
};

#endif // CHECKLISTDIALOG_H
