#ifndef PASSEDIT_H
#define PASSEDIT_H

#include <QWidget>
#include <QLineEdit>
#include <QAction>

class PasswordEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit PasswordEdit(QWidget *parent = nullptr);

private slots:
    void togglePasswordVisibility();

private:
    QAction *toggleAction;
    bool isPasswordVisible;
    void updateToggleIcon();
};

#endif // PASSEDIT_H
