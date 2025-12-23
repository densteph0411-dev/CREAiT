#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QVariant>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>

namespace Settings
{
    // Internal helpers are inline so they can live in the header
    inline QString settingsFilePath()
    {
        return QStringLiteral("settings.json");
    }

    inline QJsonObject loadSettingsObject()
    {
        const QString path = settingsFilePath();
        QFile file(path);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // No file yet, or can't read it -> empty object
            return QJsonObject{};
        }

        const QByteArray data = file.readAll();
        file.close();

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            qDebug() << "Settings: failed to parse" << path << ":" << err.errorString();
            return QJsonObject{};
        }

        return doc.object();
    }

    inline void saveSettingsObject(const QJsonObject &obj)
    {
        const QString path = settingsFilePath();
        QFile file(path);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            qDebug() << "Settings: failed to write" << path << ":" << file.errorString();
            return;
        }

        QJsonDocument doc(obj);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }

    // Public API: load a value by key, with optional default
    inline QVariant load(const QString &key,
                         const QVariant &defaultValue = QVariant())
    {
        QJsonObject root = loadSettingsObject();

        if (!root.contains(key))
            return defaultValue;

        QJsonValue val = root.value(key);
        return val.toVariant();
    }

    // Public API: save a value by key
    inline void save(const QString &key, const QVariant &value)
    {
        QJsonObject root = loadSettingsObject();

        // Update / insert the key
        root.insert(key, QJsonValue::fromVariant(value));

        // Write back to file
        saveSettingsObject(root);
    }

    // Optional helper: get the path for debugging / logging
    inline QString filePath()
    {
        return settingsFilePath();
    }

} // namespace Settings

#endif // SETTINGS_H
