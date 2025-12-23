#ifndef VERSIONITEMWIDGET_H
#define VERSIONITEMWIDGET_H

#include <QWidget>

namespace Ui {
class VersionItemWidget;
}

class VersionItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VersionItemWidget(QWidget *parent = nullptr);
    ~VersionItemWidget();

    void setData(const QString &version, const QString &description, const QString &date);

signals:
    void openLinkClicked(const QString &version);
private:
    Ui::VersionItemWidget *ui;

    QString versionString;
};

#endif // VERSIONITEMWIDGET_H
