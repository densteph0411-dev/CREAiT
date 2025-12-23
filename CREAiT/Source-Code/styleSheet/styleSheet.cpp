#include "styleSheet.h"
#include <QDir>

StyleSheet::StyleSheet() {}
StyleSheet::~StyleSheet() {}

const QString StyleSheet::loadStyleSheet(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QString styleSheet = QLatin1String(file.readAll());

    return styleSheet;
}
