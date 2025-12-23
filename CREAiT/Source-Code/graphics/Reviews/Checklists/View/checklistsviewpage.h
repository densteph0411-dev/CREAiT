#ifndef CHECKLISTSVIEWPAGE_H
#define CHECKLISTSVIEWPAGE_H

#include <QMap>
#include <QVBoxLayout>
#include <QWidget>
#include <ui_checklistsviewpage.h>
#include <checklistitem.h>
#include <reviewscontroller.h>
#include <deletedialog.h>


class ChecklistsViewPage : public QWidget, public Ui::ChecklistsViewPage
{
    Q_OBJECT

public:
    explicit ChecklistsViewPage(QWidget *parent = nullptr);
    ~ChecklistsViewPage();

    void initializeWidgets();
    void refresh();

protected:
    void showEvent(QShowEvent* event) override;

signals:
    void requestCreatePage(int pageIndex, int cellId);

private slots:
    void clickedCreateButton();
    void editChecklist(int id);
    void deleteChecklist(ChecklistItem* item);
    void handleDelete();

private:
    QMap<int, ChecklistData> checklists;
    QVBoxLayout* containerLayout;
    DeleteDialog* deleteDialog;
    ChecklistItem* deleteItem;
};

#endif // CHECKLISTSVIEWPAGE_H
