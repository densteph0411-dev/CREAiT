#ifndef TREECOMBOBOX_H
#define TREECOMBOBOX_H

#include <QWidget>
#include <QPushButton>
#include <QTreeView>
#include <QStandardItemModel>
#include <QFrame>
#include <QVBoxLayout>

class TreeComboBox : public QWidget
{
    Q_OBJECT
public:
    explicit TreeComboBox(QWidget *parent = nullptr);
    void setModel(QStandardItemModel *model);
    void setSelectedDirID(QString id);
    QString selectedText() const;
    QString selectedDirID() const;

signals:
    void selectionChanged(const QString &text);
private:
    QPushButton *triggerButton;
    QTreeView *treeView;
    QFrame *popupFrame;
    QStandardItemModel *treeModel;
    QModelIndex selectedIndex;

private slots:
    void showPopup();
    void handleSelection(const QModelIndex &index);

};

#endif // TREECOMBOBOX_H

