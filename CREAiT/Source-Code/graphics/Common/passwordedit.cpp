#include "passwordedit.h"
#include <QIcon>

PasswordEdit::PasswordEdit(QWidget *parent)
    : QLineEdit(parent), isPasswordVisible(false)
{
    // Set initial echo mode
    setEchoMode(QLineEdit::Password);

    // Create toggle action and add it to the QLineEdit
    toggleAction = addAction(QIcon(":/images/eye_open.svg"), QLineEdit::TrailingPosition);
    toggleAction->setToolTip("Show/Hide Password");

    connect(toggleAction, &QAction::triggered, this, &PasswordEdit::togglePasswordVisibility);

    updateToggleIcon();

    // Optional: styling for rounded border
    setStyleSheet("QLineEdit { padding-right: 30px; }");
}

void PasswordEdit::togglePasswordVisibility()
{
    isPasswordVisible = !isPasswordVisible;
    setEchoMode(isPasswordVisible ? QLineEdit::Normal : QLineEdit::Password);
    updateToggleIcon();
}

void PasswordEdit::updateToggleIcon()
{
    QIcon icon = QIcon(isPasswordVisible ? ":/images/eye_close.svg" : ":/images/eye_open.svg");
    toggleAction->setIcon(icon);
}
