#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QMessageLogContext>
#include <QString>
#include <styleSheet.h>

namespace Ui {
class SplashScreen;
}

class SplashScreen : public QWidget, public StyleSheet
{
    Q_OBJECT

public:
    explicit SplashScreen(QWidget *parent = nullptr);
    ~SplashScreen();

    void showMessage(const QString &message, Qt::Alignment alignment = Qt::AlignBottom | Qt::AlignLeft);
    void appendConsoleMessage(const QString &message);
    void finish(QWidget *mainWin);

    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static SplashScreen* instance();

    enum ScaleChoice {
        Keep = 0,
        Reset = 1
    };

    // Show the scale options and block until user picks Keep or Reset.
    // Returns the chosen option.
    ScaleChoice showScaleOptionsBlocking();

public slots:
    void updateProgress(const QString &message);

private:
    Ui::SplashScreen *ui;
    static SplashScreen *s_instance;
};

#endif // SPLASHSCREEN_H

