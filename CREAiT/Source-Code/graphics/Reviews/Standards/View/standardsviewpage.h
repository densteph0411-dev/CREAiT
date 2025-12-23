#ifndef STANDARDSVIEWPAGE_H
#define STANDARDSVIEWPAGE_H

#include <QMap>
#include <QWidget>
#include "ui_standardsviewpage.h"
#include "standardcell.h"
#include <reviewscontroller.h>
#include <deletedialog.h>
#include <flowlayout.h>

class StandardsViewPage : public QWidget, public Ui::StandardsViewPage
{
    Q_OBJECT

public:
    explicit StandardsViewPage(QWidget *parent = nullptr);
    ~StandardsViewPage();

    void initializeWidgets();
    void refresh();

protected:
    void showEvent(QShowEvent* event) override;

public slots:
    void createStandard();
    void editStandard(int id);
    void deleteStandard(StandardCell* cell);
    void handleDelete();

private:
    QMap<int, StandardData> standards;
    FlowLayout* layout;
    QGridLayout* containerLayout;
    DeleteDialog* deleteDialog;
    StandardCell* deleteCell;
    QString reviewsPath;
};

#endif // STANDARDSVIEWPAGE_H
