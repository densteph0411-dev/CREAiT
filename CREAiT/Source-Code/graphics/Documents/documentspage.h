#ifndef DOCUMENTSPAGE_H
#define DOCUMENTSPAGE_H

#include <QWidget>
#include <QFileDialog>
#include <QFileInfo>

#include "ui_documentspage.h"
#include <flowlayout.h>

class DocumentsPage : public QWidget, public Ui::DocumentsPage
{
    Q_OBJECT

public:
    explicit DocumentsPage(QWidget *parent = nullptr);
    ~DocumentsPage();

    void markDirty();
    void ensureRefreshed();

private:
    void addLayout(const QFileInfo& fileInfo);

private slots:
    void uploadButtonClicked();
    void refresh();

private:
    QString documentsPath;
    FlowLayout* layout;
    bool needsRefresh;
};

#endif // DOCUMENTSPAGE_H
