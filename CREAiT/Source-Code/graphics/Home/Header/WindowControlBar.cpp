#include "windowcontrolbar.h"
#include <QSizePolicy>

WindowControlBar::WindowControlBar(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    // Connect button signals to slots
    connect(aiChatButton, &QToolButton::clicked, this, &WindowControlBar::clickedAIChat);
    connect(zoomButton, &QToolButton::clicked, this, &WindowControlBar::clickedZoom);
    connect(userButton, &QToolButton::clicked, this, &WindowControlBar::clickedUser);

    aiChatButton->installEventFilter(this);
    zoomButton->installEventFilter(this);
    userButton->installEventFilter(this);

    // Even if this widget is hidden, keep allocating its size
    QSizePolicy sp = aiChatButton->sizePolicy();
    sp.setRetainSizeWhenHidden(true);
    aiChatButton->setSizePolicy(sp);
}

WindowControlBar::~WindowControlBar() {

}

bool WindowControlBar::eventFilter(QObject *obj, QEvent *event) {
    if (obj == userButton) {
        if (event->type() == QEvent::Enter) {
            userButton->setIcon(QIcon(":/images/window_user_hover.svg"));
        } else if (event->type() == QEvent::Leave) {
            userButton->setIcon(QIcon(":/images/window_user.svg"));
        }
    } else if (obj == zoomButton) {
        if (event->type() == QEvent::Enter) {
            zoomButton->setIcon(QIcon(":/images/zoom-in_hover.svg"));
        } else if (event->type() == QEvent::Leave) {
            zoomButton->setIcon(QIcon(":/images/zoom-in.svg"));
        }
    } else if (obj == aiChatButton) {
        if (event->type() == QEvent::Enter) {
            aiChatButton->setIcon(QIcon(":/images/window_aichat_hover.svg"));
        } else if (event->type() == QEvent::Leave) {
            aiChatButton->setIcon(QIcon(":/images/window_aichat.svg"));
        }
    }

    return QWidget::eventFilter(obj, event);
}

void WindowControlBar::clickedAIChat() {
    // Handle menu button click
    emit handleAIChat();
}

void WindowControlBar::clickedZoom() {
    // Handle avatar button click
    emit handleZoom();
}

void WindowControlBar::clickedUser() {
    // Handle avatar button click
    emit handleUser();
}
