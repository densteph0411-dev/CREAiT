#include "trainingdetailtablecell.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QPixmap>
#include <QLabel>

TrainingDetailTableCell::TrainingDetailTableCell(
    ContentType type,
    const QStringList &content,
    const QStringList &borders,
    const QString &iconPath,
    const QString &cornerStyle,
    QWidget *parent)
    : QFrame(parent)
{
    setFrameShape(QFrame::Box);
    setLineWidth(1);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QString borderStyle;
    for (const QString &side : borders) {
        borderStyle += QString("border-%1: 1px solid #34343426; ").arg(side);
    }

    QString baseStyle;
    // Dynamic stylesheet depending on type
    if (type == Header) {
        baseStyle = R"(
            QFrame {
                background-color: #EEF4F6;
            }
            QLabel {
                border: none;
                font-size: 18px;
                font-weight: 600;
                color: #1A1A1A;
                padding: 8px 20px;
            }
        )";
    } else {
        baseStyle = R"(
            QFrame {
                background-color: white;
            }
            QLabel {
                border: none;
                font-size: 16px;
                color: #4D4D4D;
                padding: 4px 8px;
            }
        )";
    }

    if (!cornerStyle.isEmpty()) {
        baseStyle.replace("QFrame {", "QFrame { " + cornerStyle);
    }

    if (!borderStyle.isEmpty()) {
        baseStyle.replace("QFrame {", "QFrame { " + borderStyle);
    }

    setStyleSheet(baseStyle);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(5);
    mainLayout->setAlignment(Qt::AlignTop);

    if (type == Header) {
        QLabel *label = new QLabel(content.join(" "));
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        mainLayout->addWidget(label);
    } else if (type == Text) {
        QLabel *label = new QLabel(content.join(" "));
        label->setWordWrap(true);
        mainLayout->addWidget(label);
    } else if (type == File) {
        QHBoxLayout *fileLayout = new QHBoxLayout;
        QLabel *iconLabel = new QLabel;
        if (!iconPath.isEmpty())
            iconLabel->setPixmap(QPixmap(iconPath).scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        fileLayout->addWidget(iconLabel);
        fileLayout->addWidget(new QLabel(content[0]));
        fileLayout->addStretch(1);
        mainLayout->addLayout(fileLayout);
        mainLayout->addWidget(new QLabel("3.00 MB"));
    } else if (type == Records) {
        for (const QString &record : content) {
            QHBoxLayout *recordLayout = new QHBoxLayout;
            QLabel *iconLabel = new QLabel;
            if (!iconPath.isEmpty())
                iconLabel->setPixmap(QPixmap(iconPath).scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            recordLayout->addWidget(iconLabel);
            recordLayout->addWidget(new QLabel(record));
            recordLayout->addStretch();
            mainLayout->addLayout(recordLayout);
        }
    }
}

TrainingDetailTableCell::~TrainingDetailTableCell()
{

}
