#include "contextmenu.h"

ContextMenu::ContextMenu(bool edit) {
    QAction *editAction = addAction(edit ? "Edit" : "Export");
    QWidgetAction *deleteWidgetAction = new QWidgetAction(this);
    QLabel *deleteLabel = new QLabel("Delete");
    deleteLabel->setObjectName("deleteAction");

    deleteWidgetAction->setDefaultWidget(deleteLabel);
    addAction(deleteWidgetAction);

    connect(editAction, &QAction::triggered, this, &ContextMenu::requestEdit);
    connect(deleteWidgetAction, &QWidgetAction::triggered, this, &ContextMenu::requestDelete);

    setStyleSheet(R"(
        QMenu {
            font-family: Open Sans;
            font-weight: 500;
            font-size: 14px;
            line-height: 22px;
            letter-spacing: 0%;
            color: #1A1A1A;
            background-color: white;
            border: 1px solid #ccc;
            border-radius: 8px;
            padding: 12px;
        }
        QMenu::item {
            padding: 6px 12px;
            border-radius: 6px;
        }
        QMenu::item:selected {
            background-color: #f0f0f0;
        }

        #deleteAction {
            color: #EC5E58;
            font-family: Open Sans;
            font-weight: 500;
            font-size: 14px;
            line-height: 22px;
            letter-spacing: 0%;
            padding: 6px 12px;
        }

        #deleteAction:hover,
        #deleteAction:pressed {
            padding: 6px 12px;
            border-radius: 6px;
            background-color: #f0f0f0;
        }
    )");
}
