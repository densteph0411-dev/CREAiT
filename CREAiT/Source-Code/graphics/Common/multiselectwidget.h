#ifndef MULTISELECTWIDGET_H
#define MULTISELECTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QScrollArea>
#include <QFrame>
#include <QToolButton>
#include "FlowLayout.h"

class MultiSelectWidget : public QWidget {
    Q_OBJECT
public:
    explicit MultiSelectWidget(QWidget *parent = nullptr);

    void addItem(const QString& item);
    void addItems(const QStringList& items);
    void setCurrentItem(const QString& item);
    void setCurrentItems(const QStringList& items);
    QStringList selectedItems() const;
    void clear();
    void clearSelections();

private:
    QString shortText(const QString& text, int length = 11);

private slots:
    void showPopup();
    void onCheckboxToggled(bool checked);
    void removePill(const QString &text);
    void updatePopupHeight();
    void loadStyleSheet();
    void createPopup();
    void addPill(const QString &text);

private:
    QVBoxLayout *mainLayout;
    QWidget *pillContainer;
    FlowLayout *pillLayout;
    QToolButton *dropdownButton;
    QFrame *popup;
    QVBoxLayout *popupLayout;
    QStringList items;
    QMap<QString, QCheckBox*> checkboxes;
    QScrollArea* scrollArea;
    QWidget* scrollContent;
};

#endif // MULTISELECTWIDGET_H
