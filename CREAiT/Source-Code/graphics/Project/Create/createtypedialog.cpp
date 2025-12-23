#include "createtypedialog.h"
#include <QFile>
#include <QLineEdit>
#include <QPainter>
#include <QShowEvent>
#include <QApplication>

CreateTypeDialog::CreateTypeDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    // Same window flags / behavior
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(true);

    // Prefix combo setup
    prefixCombo->setEditable(true);
    if (QLineEdit *le = qobject_cast<QLineEdit *>(prefixCombo->lineEdit())) {
        le->setPlaceholderText(tr("Select prefix here"));
    }

    connect(cancelButton, &QPushButton::clicked,
            this, &CreateTypeDialog::reject);
    connect(closeButton, &QToolButton::clicked,
            this, &CreateTypeDialog::reject);
    connect(createButton, &QPushButton::clicked,
            this, &CreateTypeDialog::handleCreateClicked);
}

CreateTypeDialog::~CreateTypeDialog()
{
}

void CreateTypeDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 100));
}

void CreateTypeDialog::showEvent(QShowEvent *event)
{
    QWidget *mainWindow = QApplication::activeWindow();
    if (mainWindow) {
        QRect parentRect = mainWindow->rect();
        QPoint topLeft = mainWindow->mapToGlobal(parentRect.topLeft());
        setFixedSize(parentRect.size());
        move(topLeft);
    }

    QDialog::showEvent(event);
}

QString CreateTypeDialog::typeName() const
{
    return typeNameEdit->text().trimmed();
}

QString CreateTypeDialog::prefix() const
{
    return prefixCombo->currentText().trimmed();
}

void CreateTypeDialog::setExistingPrefixes(const QStringList &prefixes)
{
    prefixCombo->clear();
    prefixCombo->addItems(prefixes);
}

void CreateTypeDialog::handleCreateClicked()
{
    const QString name = typeName();
    const QString pref = prefix();

    // Simple validation: both fields required
    if (name.isEmpty() || pref.isEmpty()) {
        // (Optional) add visual error states here.
        return;
    }

    accept();
}
