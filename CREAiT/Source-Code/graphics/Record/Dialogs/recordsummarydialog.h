#ifndef RECORDSUMMARYDIALOG_H
#define RECORDSUMMARYDIALOG_H

#include <QDialog>

namespace Ui {
class RecordSummaryDialog;
}

class RecordSummaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecordSummaryDialog(QWidget *parent = nullptr, const QString& recID="");
    ~RecordSummaryDialog();
public slots:
    void onCloseButtonClicked();
protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    Ui::RecordSummaryDialog *ui;

    QString m_recordID;
};

#endif // RECORDSUMMARYDIALOG_H
