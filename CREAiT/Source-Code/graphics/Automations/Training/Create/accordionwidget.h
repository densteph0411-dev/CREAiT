#ifndef ACCORDIONWIDGET_H
#define ACCORDIONWIDGET_H

#include <QWidget>
#include "ui_accordionwidget.h"
#include <texteditor.h>
#include <QComboBox>
#include "dropfilewidget.h"

class AccordionWidget : public QWidget, public Ui::AccordionWidget
{
    Q_OBJECT

public:
    explicit AccordionWidget(const int &id, const QString &title, QWidget *parent = nullptr);
    ~AccordionWidget();

    QStringList exampleItemList() const;
    void initializeWidgets();
    void hideAllSubExampleInputWidgets();
    void hideAllSubExampleResultWidgets();
    int id() const;
    void updateStyle();

signals:
    void requestDelelte(int id);

private slots:
    void clickedToggleButton();
    void clickedDeleteButton();
    void changedExampleInput(int index);
    void changedExampleResult(int index);

private:
    int index;
    bool expanded;

    enum ExampleType {
        TestCase,
        SystemRequirement,
        SoftwareRequirement,
        TestSet,
        TestProcedure,
        FreeText,
        FileUpload
    };

    QComboBox* exampleInputParamSelect;
    TextEditor* textEditor;
    DropFileWidget* exampleInputDropFileWidget;

    QComboBox* exampleResultParamSelect;
    DropFileWidget* exampleResultDropFileWidget;
};

#endif // ACCORDIONWIDGET_H
