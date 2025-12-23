#include "splashscreen.h"
#include "ui_splashscreen.h"
#include <QApplication>
#include <QScreen>
#include <QDateTime>
#include <QScrollBar>
#include <QEventLoop>

SplashScreen* SplashScreen::s_instance = nullptr;

SplashScreen::SplashScreen(QWidget *parent)
    : QWidget(parent, Qt::SplashScreen | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint)
    , ui(new Ui::SplashScreen)
{
    ui->setupUi(this);
    s_instance = this;

    ui->scaleContainer->hide();

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setStyleSheet(loadStyleSheet(":/stylesheets/SplashScreen.qss"));

    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    move(x, y);

    ui->statusLabel->setText("Initializing CREAiT...");
    ui->consoleOutput->setReadOnly(true);
}

SplashScreen::~SplashScreen()
{
    s_instance = nullptr;
    delete ui;
}

void SplashScreen::showMessage(const QString &message, Qt::Alignment alignment)
{
    Q_UNUSED(alignment);
    ui->statusLabel->setText(message);
    QApplication::processEvents();
}

void SplashScreen::appendConsoleMessage(const QString &message)
{
    ui->consoleOutput->append(message);
    QScrollBar *scrollBar = ui->consoleOutput->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
    QApplication::processEvents();
}

void SplashScreen::finish(QWidget *mainWin)
{
    Q_UNUSED(mainWin);
    close();
}

void SplashScreen::updateProgress(const QString &message)
{
    showMessage(message);
    appendConsoleMessage(QString("[%1] %2")
                         .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                         .arg(message));
}

SplashScreen* SplashScreen::instance()
{
    return s_instance;
}

void SplashScreen::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString formattedMessage;
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    
    switch (type) {
    case QtDebugMsg:
        formattedMessage = QString("[%1] DEBUG: %2").arg(timestamp, msg);
        break;
    case QtInfoMsg:
        formattedMessage = QString("[%1] INFO: %2").arg(timestamp, msg);
        break;
    case QtWarningMsg:
        formattedMessage = QString("[%1] WARNING: %2").arg(timestamp, msg);
        break;
    case QtCriticalMsg:
        formattedMessage = QString("[%1] CRITICAL: %2").arg(timestamp, msg);
        break;
    case QtFatalMsg:
        formattedMessage = QString("[%1] FATAL: %2").arg(timestamp, msg);
        break;
    }

    fprintf(stderr, "%s\n", formattedMessage.toLocal8Bit().constData());

    if (s_instance) {
        s_instance->appendConsoleMessage(formattedMessage);
    }
}

SplashScreen::ScaleChoice SplashScreen::showScaleOptionsBlocking()
{
    // show the UI elements
    ui->scaleContainer->show();

    QEventLoop loop;
    ScaleChoice result = Keep;

    // Store connections so we can disconnect them when one fires
    QMetaObject::Connection connReset;
    QMetaObject::Connection connKeep;

    connReset = connect(ui->resetButton, &QPushButton::clicked, this, [&]() {
        result = Reset;
        // disconnect both handlers to avoid dangling slots
        disconnect(connReset);
        disconnect(connKeep);
        loop.quit();
    });

    connKeep = connect(ui->keepButton, &QPushButton::clicked, this, [&]() {
        result = Keep;
        disconnect(connReset);
        disconnect(connKeep);
        loop.quit();
    });

    // Run local event loop until a button is pressed
    loop.exec();

    // Hide controls after choice is made - keep splash visible if needed
    ui->scaleContainer->hide();

    return result;
}
