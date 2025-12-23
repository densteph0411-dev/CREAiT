#ifndef SPINNERWIDGET_H
#define SPINNERWIDGET_H

#include <QWidget>
#include <QTimer>

class SpinnerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpinnerWidget(QWidget *parent = nullptr);
    void start();
    void stop();
    bool isSpinning() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTimer *timer;
    int angle;
    bool spinning;

private slots:
    void updateSpinner();
};

#endif // SPINNERWIDGET_H