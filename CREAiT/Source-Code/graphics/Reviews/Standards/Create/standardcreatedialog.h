#ifndef STANDARDCREATEDIALOG_H
#define STANDARDCREATEDIALOG_H

#include <QDialog>
#include "ui_standardcreatedialog.h"

class StandardCreateDialog : public QDialog, public Ui::StandardCreateDialog
{
    Q_OBJECT

public:
    explicit StandardCreateDialog(QWidget *parent = nullptr);
    ~StandardCreateDialog();

    void initializeWidgets();
    void setData(int id);

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void handleFileSelect(const QString& filename);

    void clickedCloseButton();
    void clickedCancelButton();
    void clickedUploadButton();

private:
    int index;
    QString reviewsPath;
    QString originFielPath;
};

#endif // STANDARDCREATEDIALOG_H
