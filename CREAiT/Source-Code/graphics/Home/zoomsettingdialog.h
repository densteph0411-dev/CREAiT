#ifndef ZOOMSETTINGDIALOG_H
#define ZOOMSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class ZoomSettingDialog;
}

class ZoomSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ZoomSettingDialog(QWidget *parent = nullptr);
    ~ZoomSettingDialog();

public slots:
    void clickedCloseButton();
    void clickedApplyButton();
protected:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    Ui::ZoomSettingDialog *ui;
};

#endif // ZOOMSETTINGDIALOG_H
