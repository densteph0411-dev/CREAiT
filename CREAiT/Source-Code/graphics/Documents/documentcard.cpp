#include "documentcard.h"
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include <contextmenu.h>

DocumentCard::DocumentCard(const QFileInfo& fileInfo, QWidget *parent) :
    QWidget(parent),
    m_fileInfo(fileInfo)
{
#ifdef Q_OS_WIN
    QNtfsPermissionCheckGuard permissionGuard;
#endif

    setupUi(this);

    QString iconPath = ":/images/Documents/documentTypes/" + fileInfo.suffix().toLower() + ".svg";
    if (!QFile::exists(iconPath)) {
        iconPath = ":/images/Documents/documentTypes/bin.svg";
    }

    QPixmap pixmap(iconPath);
    if (pixmap.isNull()) {
        qWarning() << "[setDocumentData] Failed to load icon from path:" << iconPath;
    } else {
        pixmap = pixmap.scaled(documentType->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        documentType->setPixmap(pixmap);
    }

    setElidedText(documentTitle, m_fileInfo.fileName());
    date->setText(fileInfo.birthTime().toString("MMMM d, yyyy"));
    owner->setText(fileInfo.owner());

    documentType->installEventFilter(this);

    connect(menuButton, &QToolButton::clicked, this, &DocumentCard::showMenu);
}

DocumentCard::~DocumentCard() {

}

const QFileInfo& DocumentCard::fileInfo() const {
    return m_fileInfo;
}

void DocumentCard::showMenu() {
    ContextMenu* menu = new ContextMenu(false);

    connect(menu, &ContextMenu::requestEdit, this, &DocumentCard::handleExport);
    connect(menu, &ContextMenu::requestDelete, this, &DocumentCard::handleDelete);

    QSize menuSize = menu->sizeHint();
    QPoint buttonPos = menuButton->mapToGlobal(QPoint(menuButton->width(), menuButton->height()));

    QPoint menuPos(buttonPos.x() - menuSize.width(), buttonPos.y());
    menu->exec(menuPos);
}

void DocumentCard::handleDelete() {
    QString filePath = m_fileInfo.absoluteFilePath();
    if(!filePath.isEmpty()) {
        if(!QFile::remove(filePath)) {
            qWarning() << "[handleDelete] Failed to remove file:" << filePath;
        }
    }

    emit requestRefresh();
}

void DocumentCard::handleExport() {
    QString filePath = m_fileInfo.absoluteFilePath();

    if (filePath.isEmpty() || !QFile::exists(filePath)) {
        qWarning() << "[handleExport] Invalid or missing file:" << filePath;
        return;
    }

    QString suggestedName = m_fileInfo.fileName();

    QString exportPath = QFileDialog::getSaveFileName(
        this,
        tr("Export Document"),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + suggestedName,
        tr("All Files (*)")
        );

    if (exportPath.isEmpty())
        return;

    QMessageBox msgBox;

    if (!QFile::copy(filePath, exportPath)) {
        qWarning() << "[handleExport] Failed to export to:" << exportPath;
    } else {
        qDebug() << "[handleExport] File exported to:" << exportPath;
    }
}

void DocumentCard::setElidedText(QLabel* label, const QString &text) {
    QFontMetrics metrics(label->font());

    int availableWidth = label->contentsRect().width();

    QString clippedText = metrics.elidedText(text, Qt::ElideRight, availableWidth);
    label->setText(clippedText);
}

bool DocumentCard::eventFilter(QObject *obj, QEvent *event) {
    if(obj == documentType && event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if(mouseEvent->button() == Qt::LeftButton) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(m_fileInfo.absoluteFilePath()));
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}

