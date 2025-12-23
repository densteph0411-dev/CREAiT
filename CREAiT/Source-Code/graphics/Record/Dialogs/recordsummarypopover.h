#ifndef RECORDSUMMARYPOPOVER_H
#define RECORDSUMMARYPOPOVER_H

#include <QWidget>

namespace Ui {
class RecordSummaryPopover;
}

class RecordSummaryPopover : public QWidget
{
    Q_OBJECT

public:
    static RecordSummaryPopover* instance(QWidget *parent = nullptr);
    void loadRecord(const QString &recID);
    void showAt(const QPoint &globalPos);
    void hidePopover();
    void hideIfNotHovered();

protected:
    void leaveEvent(QEvent *event) override;

private:
    explicit RecordSummaryPopover(QWidget *parent = nullptr);
    ~RecordSummaryPopover();

    Ui::RecordSummaryPopover *ui;

    static RecordSummaryPopover *s_instance;
};

#endif // RECORDSUMMARYPOPOVER_H
