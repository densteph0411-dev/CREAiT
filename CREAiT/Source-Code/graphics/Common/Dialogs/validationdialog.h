#ifndef VALIDATIONDIALOG_H
#define VALIDATIONDIALOG_H

#include <QDialog>
#include "ui_validationdialog.h"

class ValidationDialog : public QDialog, public Ui::ValidationDialog
{
    Q_OBJECT

public:
    explicit ValidationDialog(const QString& title = "Validation Error", 
                             const QString& message = "", 
                             QWidget *parent = nullptr);
    ~ValidationDialog();
    
    void setMessage(const QString& message);

public slots:
    void onOkButtonClicked();
    void onCloseButtonClicked();

protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);
};

#endif // VALIDATIONDIALOG_H

