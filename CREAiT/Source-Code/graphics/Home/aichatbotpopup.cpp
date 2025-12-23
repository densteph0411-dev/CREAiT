#include "aichatbotpopup.h"
#include "ui_aichatbotpopup.h"
#include <QDate>
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <QScrollBar>
#include "flowlayout.h"

AIChatBotPopup::AIChatBotPopup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AIChatBotPopup)
{
    ui->setupUi(this);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 80));  // soft semi-transparent black

    this->setGraphicsEffect(shadow);
    this->setAttribute(Qt::WA_TranslucentBackground);  // important for shadow rendering
    this->setStyleSheet("background-color: white; border-radius: 12px;");

    flowLayout = new FlowLayout(ui->suggestionGroup, 0, 10, 10);
    ui->suggestionGroup->setLayout(flowLayout);

    QStringList suggestions = {
                         "Lorem ipsum faucibus libero consectetu?", "Lorem ips?",
                         "Lorem ferm bus?", "Lorem ipsum?", "Lorem f ipsum?", "Lorem ipsum?", "Lorem f ipsum?" };
    setSuggesionList(suggestions);
}

AIChatBotPopup::~AIChatBotPopup()
{
    delete ui;
}

QWidget* AIChatBotPopup::createMessageBubble(const QString &text, bool fromBot) {
    QWidget *container = new QWidget;
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setSpacing(2);
    containerLayout->setContentsMargins(0, 0, 0, 0);

    // Optional: Add icon + bubble
    QWidget *bubble = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(bubble);
    layout->setContentsMargins(10, 5, 10, 5);

    QLabel *iconLabel = new QLabel;
    iconLabel->setFixedSize(38, 38);
    iconLabel->setPixmap(QPixmap(fromBot ? ":/images/Project/ai-chat-bot.svg" : ":/images/Project/ai-chat-user.svg").scaled(38, 38));

    QLabel *label = new QLabel(text);
    label->setWordWrap(true);

    label->setStyleSheet(R"(
            background-color: #EEF4F6;
            border-radius: 8px;
            padding: 8px;
            font-family: Open Sans;
            font-weight: 500;
            font-size: 16px;
            line-height: 24px;
            letter-spacing: 0%;
            color: #1A1A1A;
    )");
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    if (fromBot) {
        layout->addWidget(iconLabel);
        layout->addWidget(label);
        layout->addStretch();
    } else {
        layout->addStretch();
        layout->addWidget(label);
        layout->addWidget(iconLabel);
    }

    containerLayout->addWidget(bubble);

    // Timestamp
    QLabel *timestamp = new QLabel(QDateTime::currentDateTime().toString("hh:mm ap"));
    timestamp->setStyleSheet("color: gray; font-size: 10px;");
    timestamp->setAlignment(fromBot ? Qt::AlignLeft : Qt::AlignRight);
    containerLayout->addWidget(timestamp);

    return container;
}

void AIChatBotPopup::onSendClicked() {
    QString userText = ui->questionInput->text().trimmed();
    if (!userText.isEmpty()) {
        addMessage(userText, false);  // user message
        ui->questionInput->clear();

        // Simulated bot response
        QTimer::singleShot(500, [=]() {
            addMessage("Bot response to: " + userText, true);
        });
    }
}

void AIChatBotPopup::onSuggestionClicked() {
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (btn) {
        ui->questionInput->setText(btn->text());
        onSendClicked();
    }
}

void AIChatBotPopup::onRefreshToolClicked() {

}

void AIChatBotPopup::onCloseToolClicked() {
    close();
}

void AIChatBotPopup::addMessage(const QString &text, bool fromBot) {
    static QString lastDateShown;
    QString currentDate = QDate::currentDate().toString("MMMM d, yyyy");

    if (currentDate != lastDateShown) {
        QLabel *dateDivider = new QLabel(currentDate);
        dateDivider->setAlignment(Qt::AlignCenter);
        dateDivider->setStyleSheet("color: gray; font-weight: bold; padding: 4px;");
        ui->scrollAreaWidgetContents->layout()->addWidget(dateDivider);
        lastDateShown = currentDate;
    }

    ui->scrollAreaWidgetContents->layout()->addWidget(createMessageBubble(text, fromBot));

    QTimer::singleShot(0, [this]() {
        ui->chatScrollArea->verticalScrollBar()->setValue(ui->chatScrollArea->verticalScrollBar()->maximum());
    });
}

void AIChatBotPopup::setSuggesionList(const QStringList& suggestions) {
    // Remove all existing widgets in flowLayout
    while (QLayoutItem* item = flowLayout->takeAt(0)) {
        QWidget *widget = item->widget();
        if (widget) {
            flowLayout->removeWidget(widget);
            widget->deleteLater();  // safely deletes after event loop
        }
        delete item;  // delete the layout item itself
    }

    for (const QString &text : suggestions) {
        QPushButton *btn = new QPushButton(text);
        btn->setStyleSheet(R"(
            QPushButton {
                border: 1px solid #50256C;
                border-radius: 16px;
                padding: 4px 12px;
                color: #50256C;
                background-color: transparent;
                font-family: Open Sans;
                font-weight: 500;
                font-size: 16px;
                line-height: 24px;
                letter-spacing: 0%;
                text-align: center;
            }
            QPushButton:hover {
                background-color: #f5f0ff;
            }
        )");
        connect(btn, SIGNAL(clicked()), this, SLOT(onSuggestionClicked()));
        flowLayout->addWidget(btn);
    }
}
