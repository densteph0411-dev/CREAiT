#include "reviewitemcheckresultdialog.h"
#include "reviewitemcheckresultwidget.h"
#include <hseparatorwidget.h>
#include <QPainter>

ReviewItemCheckResultDialog::ReviewItemCheckResultDialog(QWidget *parent)
    : QDialog(parent)
{
    initializeWidgets();
}

ReviewItemCheckResultDialog::~ReviewItemCheckResultDialog()
{

}

void ReviewItemCheckResultDialog::initializeWidgets() {
    setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(18, 18, 18, 18);
    mainLayout->setSpacing(18);

    connect(closeButton, &QToolButton::clicked, this, &QDialog::reject);
}

void ReviewItemCheckResultDialog::setData(int id) {
    const QString question = "What is the best language and the best GUI framework in desktop application?";
    int answerId = 2;
    const QString descritpion = "Of course, C++ and Qt framework! You can know easily it is rihgt even though you're a beginner in programming!";

    for (int i = 0; i < 5; ++i) {
        ReviewItemCheckResultWidget* widget = new ReviewItemCheckResultWidget(this);
        widget->setData(question, answerId, descritpion);

        if (mainLayout->count() > 0) {
            HSeparatorWidget* separator = new HSeparatorWidget(container);
            mainLayout->addWidget(separator);
        }
        mainLayout->addWidget(widget);
    }
}

void ReviewItemCheckResultDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void ReviewItemCheckResultDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    QRect parentRect = mainWindow->rect();
    QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
    setFixedSize(parentRect.size());
    move(topLeft);

    QDialog::showEvent(event);
}
