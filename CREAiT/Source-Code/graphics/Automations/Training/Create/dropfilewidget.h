#ifndef DROPFILEWIDGET_H
#define DROPFILEWIDGET_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileDialog>
#include "ui_dropfilewidget.h"

class DropFileWidget : public QWidget, public Ui::DropFileWidget
{
    Q_OBJECT

public:
    explicit DropFileWidget(QWidget *parent = nullptr);
    ~DropFileWidget();

    void initializeWidgets();
    void clear();
    void handleFile(const QString& filePath);
    QString formatFileSize(qint64 bytes) const;

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
    void requestSelectFile(const QString& filePath);

private slots:
    void clickedFileSelectButton();
    void clickedCloseButton();
};

#endif // DROPFILEWIDGET_H
