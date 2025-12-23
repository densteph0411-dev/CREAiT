#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class TestWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TestWindow(QWidget *parent = nullptr);
    ~TestWindow();

private slots:
               // add any slots here

    // The run all integration tests button was clicked
    void on_runAllIntegrationTestsButton_clicked();

private:
    Ui::Dialog *ui;
};

#endif // TESTWINDOW_H
