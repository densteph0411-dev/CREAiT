#include "logmanager.h"
#include <QCoreApplication>
#include <QDir>

LogManager::LogManager() {
    QString exeDir = QCoreApplication::applicationDirPath();
    QString logDirPath = exeDir + "/logs";
    QDir().mkpath(logDirPath);

    currentDateString = QDate::currentDate().toString("yyyy-MM-dd");
    logFilePath = QString("%1/log_%2.log").arg(logDirPath, currentDateString);
}

LogManager& LogManager::instance() {
    static LogManager _instance;
    return _instance;
}

void LogManager::writeLog(const QString& message, const QString& time) {
    QMutexLocker locker(&mutex);

    QString today = QDate::currentDate().toString("yyyy-MM-dd");
    if (today != currentDateString) {
        // Rotate
        currentDateString = today;
        QString exeDir = QCoreApplication::applicationDirPath();
        QString logDirPath = exeDir + "/logs";
        logFilePath = QString("%1/log_%2.log").arg(logDirPath, currentDateString);
    }

    QFile file(logFilePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << "[" << time << "]   " << message << "\n";
        file.close();
    }
}

QString LogManager::getLogFilePath() const  {
    return logFilePath;
}
