#ifndef CHECKLISTVIEWDIALOG_H
#define CHECKLISTVIEWDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include "ui_checklistviewdialog.h"

class ChecklistViewDialog : public QDialog, public Ui::ChecklistViewDialog
{
    Q_OBJECT

public:
    explicit ChecklistViewDialog(QWidget *parent = nullptr);
    ~ChecklistViewDialog();

    void initializeWidgets();
    void setData(const QString& title, const QStringList& quesitons);

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QVBoxLayout* mainLayout;
};

#endif // CHECKLISTVIEWDIALOG_H
