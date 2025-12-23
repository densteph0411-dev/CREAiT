#ifndef EDITRECORDFORM_H
#define EDITRECORDFORM_H

#include <QWidget>

namespace Ui {
class EditRecordForm;
}

class EditRecordForm : public QWidget
{
    Q_OBJECT

public:
    explicit EditRecordForm(QWidget *parent = nullptr);
    ~EditRecordForm();

    void initializeForm(QString recID);

public slots:
    void onSaveButtonClicked();
    void onCancelButtonClicked();

signals:
    void recordUpdated(QString id);
    void cancelEditRecord(QString id);

private:
    Ui::EditRecordForm *ui;

    QString recordID;
};

#endif // EDITRECORDFORM_H
