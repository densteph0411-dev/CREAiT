#ifndef ADDITIONALFIELDSPANEL_H
#define ADDITIONALFIELDSPANEL_H

#include <QMap>
#include <QVBoxLayout>
#include <QWidget>
#include "ui_additionalfieldspanel.h"
#include "additionalfielditem.h"


class AdditionalFieldsPanel : public QWidget, public Ui::AdditionalFieldsPanel
{
    Q_OBJECT

public:
    explicit AdditionalFieldsPanel(QWidget *parent = nullptr);
    ~AdditionalFieldsPanel();

    void initializeWidgets();
    void reset();

signals:
    void requestBack();
    void requestNext();

private slots:
    void clickedAddButton();
    void clickedBackButton();
    void clickedNextButton();
    void handleDelete(int id);

private:
    int nextId;
    QVBoxLayout* fieldLayout;
    QMap<int, AdditionalFieldItem*>* additionalFields;

};

#endif // ADDITIONALFIELDSPANEL_H
