#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QStandardItem>
#include <QTimer>
#include "recordsummarypopover.h"

class ProjectTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit ProjectTreeView(QWidget *parent = nullptr);

    QModelIndexList draggedIndexes;

    void startDrag(Qt::DropActions supportedActions) override;
    void dropEvent(QDropEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

signals:
    void actionMoreButtonClicked(const QModelIndex &index);
    void actionPlusButtonClicked(const QModelIndex &index);

    void recordMoved(QStandardItem* srcItem, QStandardItem* parentItem, int rowIndex);
private slots:
    void showPreview();
private:
    QTimer hoverTimer;
    QModelIndex hoveredIndex;
    QPoint hoverGlobalPos;
};

#endif // PROJECTTREEVIEW_H
