#ifndef VERSIONHISTORYDIALOG_H
#define VERSIONHISTORYDIALOG_H

#include <QDialog>

namespace Ui {
class VersionHistoryDialog;
}

class VersionHistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VersionHistoryDialog(QWidget *parent = nullptr, const QString& recID = "");
    ~VersionHistoryDialog();
public slots:
    void onCloseButtonClicked();
protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    Ui::VersionHistoryDialog *ui;
    QString m_recordID;
};

#endif // VERSIONHISTORYDIALOG_H
