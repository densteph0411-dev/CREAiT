// If the test flag is set, define the test window instead
#include <QApplication>

#include <mainWindow.h>
#include <homepage.h>
// Qt Core / GUI / Widgets
#include <projectcard.h>
#include <records.h>
#include <iostream>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QTranslator>
#include <QLocale>
#include <QProcess>
#include <QSystemTrayIcon>
#include <QTranslator>
#include <QThread>
#include <QIcon>

// Widgets you use directly
#include <QPushButton>
#include "AppScale.h"
#include "splashscreen.h"
#include <settings.h>

// (Optional) if you use debugging/logging in this file
#include <QDebug>

static double loadStartupScaleFactor()
{
    // Read saved percent (or -1 for Auto). No QApplication needed.
    const int savedPct = Settings::load("scalePercent", -1).toInt();

    if (savedPct == -1) {
        // Auto: compute from (design 1920x1080, screen res, Windows display scale) and snap
        AppScaleInfo scaleInfo = ComputeSnappedAppScale(1920, 1080);
        return scaleInfo.snappedFactor;
    }
    return qBound(0.5, savedPct / 100.0, 3.0);
}

int main(int argc, char *argv[])
{
    // 1) Compute snapped factor for your design baseline (1920x1080)
    double factor = loadStartupScaleFactor();
    AppScaleInfo scaleInfo = ComputeSnappedAppScale(1920, 1080); // e.g., 1.25 for 125%
    double autoFactor = scaleInfo.snappedFactor;

    // 2) Feed to Qt BEFORE constructing QApplication
    qputenv("QT_SCALE_FACTOR", QByteArray::number(factor));

    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication a(argc, argv);

#ifndef TEST_INTTEST
    SplashScreen *splash = new SplashScreen();
    splash->show();
    qInstallMessageHandler(SplashScreen::messageHandler);
    splash->updateProgress("Starting CREAiT Application...");
#endif
    splash->updateProgress("Checking display scaling...");
    if (abs(factor - autoFactor) > 0.5) {
        splash->updateProgress("Waiting for you to confirm: Keep or Reset");
        auto choice = splash->showScaleOptionsBlocking();
        if (choice == SplashScreen::Reset) {
            // Save to settings
            Settings::save("scalePercent", -1);

            // Relaunch self and exit
            const QString exe = QCoreApplication::applicationFilePath();
            const QStringList args = QCoreApplication::arguments();
            QProcess::startDetached(exe, args, QDir::currentPath());
            qApp->quit();

            return 0;
        }
    }

    QApplication::setWindowIcon(QIcon(":images/app_icon.ico"));

#ifndef TEST_INTTEST
    splash->updateProgress("Setting up application icon...");
    QThread::msleep(50);
#endif

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("System tray not available!"));
        return 1;
    }

#ifndef TEST_INTTEST
    splash->updateProgress("Checking system tray availability...");
    QThread::msleep(50);
#endif

    QApplication::setQuitOnLastWindowClosed(false);

#ifndef TEST_INTTEST
    splash->updateProgress("Loading translations...");
    QThread::msleep(50);
#endif

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "CREAiT_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

#ifndef TEST_INTTEST
    splash->updateProgress("Initializing project system...");
    QThread::msleep(50);
#endif

    qDebug() << "Initializing Data" << "\n";
    // Initialize basic required parameters
    projectInfoRecord::initialize();
    qDebug() << "Initialization Complete" << "\n";

#ifdef TEST_INTTEST
    TestWindow testWindow;
    testWindow.show();
    return a.exec();
#else

    splash->updateProgress("Loading main window...");
    QThread::msleep(100);

    qDebug() << "Starting Main Window" << "\n";
    MainWindow window;

    splash->updateProgress("Initializing main window components...");
    QThread::msleep(100);

    splash->updateProgress("CREAiT is ready!");
    QThread::msleep(500);

    splash->finish(&window);
    window.show();
    qInstallMessageHandler(nullptr);

    return a.exec();
#endif
}
