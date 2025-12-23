#include "spinnerwidget.h"
#include <QPainter>

SpinnerWidget::SpinnerWidget(QWidget *parent)
    : QWidget(parent), angle(0), spinning(false)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_OpaquePaintEvent, false);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SpinnerWidget::updateSpinner);
    timer->setInterval(100);
}

void SpinnerWidget::start()
{
    if (!spinning) {
        spinning = true;
        timer->start();
        show();
        raise();                      // Ensure it's on top
        update();                     // Immediate repaint request
        /*qApp->processEvents();   */     // 🔥 Force it to be painted immediately
    }
}

void SpinnerWidget::stop()
{
    if (spinning) {
        spinning = false;
        timer->stop();
        hide();
    }
}

bool SpinnerWidget::isSpinning() const
{
    return spinning;
}

void SpinnerWidget::updateSpinner()
{
    angle = (angle + 30) % 360;
    update();
}

void SpinnerWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int size = qMin(width(), height());
    const int rectCount = 8;
    const int radius = size / 4;

    painter.translate(width() / 2, height() / 2);
    painter.rotate(angle);

    for (int i = 0; i < rectCount; ++i) {
        QColor color = Qt::darkBlue;
        color.setAlphaF(1.0 - (i / static_cast<double>(rectCount)));
        painter.setBrush(color);
        painter.setPen(Qt::NoPen);
        painter.drawRect(radius, -size / 20, size / 8, size / 10);
        painter.rotate(360.0 / rectCount);
    }
}
