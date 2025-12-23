#include "treecombobox.h"
#include <QGraphicsDropShadowEffect>

TreeComboBox::TreeComboBox(QWidget *parent)
    : QWidget(parent)
{
    triggerButton = new QPushButton("Select parent folder here", this);
    triggerButton->setStyleSheet(R"(
        QPushButton {
            text-align: left;
            padding: 6px 12px;
            border: 1px solid #ccc;
            border-radius: 8px;
            background-color: white;
            color: #1A1A1A;
            font-family: Open Sans;
            font-weight: 400;
            font-size: 16px;
            line-height: 24px;
            letter-spacing: 0%;
        }
    )");

    triggerButton->setCursor(Qt::PointingHandCursor);
    triggerButton->setMinimumHeight(40);
    triggerButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(triggerButton);

    popupFrame = new QFrame(nullptr, Qt::Popup);
    popupFrame->setLayout(new QVBoxLayout());
    popupFrame->layout()->setContentsMargins(0, 0, 0, 0);
    popupFrame->setFixedSize(648, 300);
    popupFrame->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    popupFrame->setAttribute(Qt::WA_TranslucentBackground);

    treeView = new QTreeView(popupFrame);
    treeView->setHeaderHidden(true);
    treeView->setStyleSheet(R"(
        QTreeView {
            border-radius: 8px;
            padding-top: 17px;
            padding-right: 19px;
            padding-bottom: 17px;
            padding-left: 19px;
            border: 1px solid #ccc;
            background-color: white;
            color: #1A1A1A;
            font-family: Open Sans;
            font-weight: 500;
            font-size: 14px;
            line-height: 22px;
            letter-spacing: 0%;
            spacing: 12px;
        }

        QTreeView::item {
        }

        QTreeView::item:hover {
            border-radius: 4px;
            background-color: gray;
        }

        QScrollBar:vertical {
            background: #F0F0F0;
            width: 6px;
            margin: 0px;
            border-radius: 3px;
        }

        QScrollBar::handle:vertical {
            background: #2D1457;
            min-height: 30px;
            border-radius: 3px;
        }

        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical,
        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical {
            background: none;
        }

        QScrollBar:horizontal {
            background: #F0F0F0;
            width: 6px;
            margin: 0px;
        }

        QScrollBar::handle:horizontal {
            background: #2D1457;
            min-width: 30px;
            border-radius: 4px;
        }

        QScrollBar::add-line:horizontal,
        QScrollBar::sub-line:horizontal,
        QScrollBar::add-page:horizontal,
        QScrollBar::sub-page:horizontal {
            background: none;
        }
    )");
    popupFrame->layout()->addWidget(treeView);

    connect(triggerButton, &QPushButton::clicked, this, &TreeComboBox::showPopup);
    connect(treeView, &QTreeView::clicked, this, &TreeComboBox::handleSelection);
}

void TreeComboBox::setModel(QStandardItemModel *model)
{
    treeModel = model;
    treeView->setModel(model);
}

QString TreeComboBox::selectedText() const
{
    return triggerButton->text();
}

void TreeComboBox::showPopup()
{
    QPoint pos = triggerButton->mapToGlobal(QPoint(0, triggerButton->height()));
    popupFrame->move(pos);
    popupFrame->show();
}

void TreeComboBox::handleSelection(const QModelIndex &index)
{
    QString text = treeModel->itemFromIndex(index)->text();
    triggerButton->setText(text);
    emit selectionChanged(text);

    selectedIndex = index;
    popupFrame->hide();
}

QString TreeComboBox::selectedDirID() const
{
    if (selectedIndex.isValid()) {
        return selectedIndex.data(Qt::UserRole).toString();
    } else {
        return "PROJ";
    }
}

void TreeComboBox::setSelectedDirID(QString id)
{
    QModelIndexList matches = treeModel->match(
        treeModel->index(0, 0),             // start from root
        Qt::UserRole,              // dirID is stored here
        QVariant::fromValue(id),    // value to match
        1,                             // only one match
        Qt::MatchExactly | Qt::MatchRecursive
        );

    if (!matches.isEmpty()) {
        QModelIndex matchIndex = matches.first();
        handleSelection(matchIndex);
    } else {
        selectedIndex = QModelIndex();
        triggerButton->setText("Select parent folder here");
    }
}
