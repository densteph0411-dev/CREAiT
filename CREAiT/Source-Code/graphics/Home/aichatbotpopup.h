#ifndef AICHATBOTPOPUP_H
#define AICHATBOTPOPUP_H

#include <QWidget>
#include <flowlayout.h>

namespace Ui {
class AIChatBotPopup;
}

class AIChatBotPopup : public QWidget
{
    Q_OBJECT

public:
    explicit AIChatBotPopup(QWidget *parent = nullptr);
    ~AIChatBotPopup();

    void addMessage(const QString &text, bool fromBot);
    void setSuggesionList(const QStringList &suggestions);
public slots:
    void onRefreshToolClicked();
    void onCloseToolClicked();
    void onSendClicked();
    void onSuggestionClicked();
protected:
    QWidget* createMessageBubble(const QString &text, bool fromBot);
private:
    FlowLayout *flowLayout;
    Ui::AIChatBotPopup *ui;
};

#endif // AICHATBOTPOPUP_H
