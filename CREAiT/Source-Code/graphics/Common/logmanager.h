#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QString>
#include <QMutex>

class LogManager
{
public:
    static LogManager& instance();

    void writeLog(const QString& message, const QString& time);
    QString getLogFilePath() const;

private:
    LogManager();

private:
    QString logFilePath;
    QMutex mutex;
    QString currentDateString;
};

#endif // LOGMANAGER_H
