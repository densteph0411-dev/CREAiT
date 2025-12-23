#ifndef CREATEDIRECTORYDIALOG_H
#define CREATEDIRECTORYDIALOG_H

#include <QDialog>

namespace Ui {
class CreateDirectoryDialog;
}

class CreateDirectoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDirectoryDialog(QWidget *parent = nullptr);
    ~CreateDirectoryDialog();

    QString directoryName() const;
    QString parentID() const;
    void setParentID(const QString& parentID);
protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);
public slots:
    void onCreateButtonClicked();
    void onCancelButtonClicked();
    void onCloseButtonClicked();
private:
    Ui::CreateDirectoryDialog *ui;

    QString m_dirName;
    QString m_parentID;
};

#endif // CREATEDIRECTORYDIALOG_H
