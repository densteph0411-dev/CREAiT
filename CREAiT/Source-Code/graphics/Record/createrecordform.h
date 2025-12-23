#ifndef CREATERECORDFORM_H
#define CREATERECORDFORM_H

#include <QWidget>

namespace Ui {
class CreateRecordForm;
}

class CreateRecordForm : public QWidget
{
    Q_OBJECT

public:
    explicit CreateRecordForm(QWidget *parent = nullptr);
    ~CreateRecordForm();

    void initializeForm(const QString& parent = "PROJ", const QString& treeViewIndex = "");

public slots:
    void onSaveButtonClicked();

private:
    Ui::CreateRecordForm *ui;

    QString m_parent;
    QString m_treeViewIndex;
};

#endif // CREATERECORDFORM_H
