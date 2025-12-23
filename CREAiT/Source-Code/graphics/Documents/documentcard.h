#ifndef DOCUMENTCARD_H
#define DOCUMENTCARD_H

#include <QWidget>
#include <QDateTime>
#include <QLabel>
#include <QMouseEvent>
#include <QFileInfo>

#include "ui_documentcard.h"


class DocumentCard : public QWidget, public Ui::documentcard
{
    Q_OBJECT

public:
    explicit DocumentCard(const QFileInfo& fileInfo, QWidget *parent = nullptr);
    ~DocumentCard();

    const QFileInfo& fileInfo() const;

signals:
    void requestRefresh();

private slots:
    void showMenu();
    void handleDelete();
    void handleExport();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void setElidedText(QLabel* label, const QString &text);

private:
    QFileInfo m_fileInfo;
};

#endif // DOCUMENTCARD_H
