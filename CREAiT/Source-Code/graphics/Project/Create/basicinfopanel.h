#ifndef BASICINFOPANEL_H
#define BASICINFOPANEL_H

#include <QWidget>
#include "ui_basicinfopanel.h"

class BasicInfoPanel : public QWidget, public Ui::BasicInfoPanel
{
    Q_OBJECT

public:
    explicit BasicInfoPanel(QWidget *parent = nullptr);
    ~BasicInfoPanel();

    void reset();
    
    QString getProjectName() const;
    QString getDescription() const;
    QString getProjectFilePath() const;
    QString getCredentialFileName() const;
    QString getRecordPrefix() const;
    QStringList getSelectedRecordTypes() const;

signals:
    void requestNext();

private slots:
    void clickedNextButton();
    void clickedProjectFilePathBrowseButton();
    void clickedRecordTypeAddButton();

private:
    void loadRecordTypesFromData();

    QString m_recordTypeSelectDefaultStyle;
};

#endif // BASICINFOPANEL_H
