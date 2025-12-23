#ifndef ADDDOCUMENTDIALOG_H
#define ADDDOCUMENTDIALOG_H

#include <QDialog>
#include "ui_adddocumentdialog.h"

class AddDocumentDialog : public QDialog, public Ui::AddDocumentDialog
{
    Q_OBJECT

public:
    explicit AddDocumentDialog(QWidget *parent = nullptr);
    ~AddDocumentDialog();
    
    QString getDocumentName() const;
    QString getPurpose() const;
    QString getDescription() const;

public slots:
    void onSaveButtonClicked();
    void onCancelButtonClicked();
    void onCloseButtonClicked();

protected:
    void showEvent(QShowEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
};

#endif // ADDDOCUMENTDIALOG_H

