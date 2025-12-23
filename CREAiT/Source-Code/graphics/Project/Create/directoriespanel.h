#ifndef DIRECTORIESPANEL_H
#define DIRECTORIESPANEL_H

#include <QWidget>
#include "ui_directoriespanel.h"
#include <QPointer>

class BasicInfoPanel;

class DirectoriesPanel : public QWidget, public Ui::DirectoriesPanel
{
    Q_OBJECT

public:
    explicit DirectoriesPanel(QWidget *parent = nullptr);
    ~DirectoriesPanel();

    void initializeWidgets();
    void reset();
    
    QString getBaseDirectory() const;
    QString getProgramDirectory() const;
    QString getProjectDirectory() const;

    void setBasicInfoPanel(BasicInfoPanel* panel);

private:
    QPointer<BasicInfoPanel> m_basicInfoPanel;

signals:
    void requestBack();
    void requestNext();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void clickedBackButton();
    void clickedNextButton();
};

#endif // DIRECTORIESPANEL_H
