#ifndef HEADER_H
#define HEADER_H

#include <QWidget>

#include "ui_header.h"

#include <styleConstants.h>

class Header : public QWidget, public Ui::Header
{
    Q_OBJECT

public:
    explicit Header(QWidget *parent = nullptr);
    ~Header();

    void addItemtoAddressBar(const QString& projectName);

signals:
    void handleAIChat();
    void handleUser();
    void handleZoom();
    void handleTabNavigation(int index, const QString& name);
    void handleAddressBarItemClose(int index, const QString& name);
};

#endif // HEADER_H
