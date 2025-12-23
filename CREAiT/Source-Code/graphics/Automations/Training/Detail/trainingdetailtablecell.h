#ifndef TRAININGDETAILTABLECELL_H
#define TRAININGDETAILTABLECELL_H

#include <QWidget>
#include <QFrame>
#include <QStringList>

class TrainingDetailTableCell : public QFrame
{
    Q_OBJECT

public:
    enum ContentType { Header, Text, File, Records };

    explicit TrainingDetailTableCell(
        ContentType type,
        const QStringList &content,
        const QStringList &borders,
        const QString &iconPath = "",
        const QString &cornerStyle = "",
        QWidget *parent = nullptr);
    ~TrainingDetailTableCell();
};

#endif // TRAININGDETAILTABLECELL_H
