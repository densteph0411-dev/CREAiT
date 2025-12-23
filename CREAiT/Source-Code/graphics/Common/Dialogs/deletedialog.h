#ifndef DELETEDIALOG_H
#define DELETEDIALOG_H

#include <QDialog>
#include "ui_deletedialog.h"

class DeleteDialog : public QDialog, public Ui::DeleteDialog
{
    Q_OBJECT

public:
    explicit DeleteDialog(const QString& title = "", QWidget *parent = nullptr);
    ~DeleteDialog();

signals:
    void requestDelete();

public slots:
    void onDeleteButtonClicked();
    void onCancelButtonClicked();
    void onCloseButtonClicked();

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);
};

#endif // DELETEDIALOG_H
