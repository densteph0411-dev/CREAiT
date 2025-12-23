#ifndef CREATETYPEDIALOG_H
#define CREATETYPEDIALOG_H

#include <QDialog>
#include "ui_createtypedialog.h"

class CreateTypeDialog : public QDialog, public Ui::CreateTypeDialog
{
    Q_OBJECT

public:
    explicit CreateTypeDialog(QWidget *parent = nullptr);
    ~CreateTypeDialog();

    QString typeName() const;
    QString prefix() const;
    void setExistingPrefixes(const QStringList &prefixes);

private slots:
    void handleCreateClicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
};

#endif // CREATETYPEDIALOG_H
