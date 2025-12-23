#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QString>

class StyleSheet
{
public:
    StyleSheet();
    virtual ~StyleSheet();
protected:
	const QString loadStyleSheet(const QString& filename);
};
#endif // STYLESHEET_H
