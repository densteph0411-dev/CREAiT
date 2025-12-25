#include "directoryselectitem.h"
#include "records.h"
#include <QFileDialog>

DirectorySelectItem::DirectorySelectItem(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    connect(browseButton, &QPushButton::clicked, this, &DirectorySelectItem::clickedDirectorySelectButton);
}

DirectorySelectItem::~DirectorySelectItem()
{

}

// void DirectorySelectItem::clickedDirectorySelectButton() {
//     QString dir = QFileDialog::getExistingDirectory(
//         this,
//         tr("Select Directory"),
//         "",
//         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
//         );

//     if (!dir.isEmpty()) {
//         directoryPathEdit->setText(dir); // Example
//     }
// }

void DirectorySelectItem::clickedDirectorySelectButton()
{
    QString startDir = directoryPathEdit->text().trimmed();

    // If empty, use fallback (optional: stored path)
    if (startDir.isEmpty()) {
        startDir = QDir::homePath();
    }

    // Normalize path
    startDir = QDir::cleanPath(startDir);

    // If it's a file path, use its parent folder
    QFileInfo fi(startDir);
    if (fi.exists() && fi.isFile()) {
        startDir = fi.absolutePath();
    }

    // If directory doesn't exist, fallback
    if (!QDir(startDir).exists()) {
        startDir = QDir::homePath();
    }

    QString selectedDir = QFileDialog::getExistingDirectory(
        this,
        tr("Select Directory"),
        startDir,
        QFileDialog::ShowDirsOnly
    );

    if (!selectedDir.isEmpty()) {
        directoryPathEdit->setText(QDir::cleanPath(selectedDir));
    }
}


void DirectorySelectItem::reset() {
    directoryPathEdit->clear();
    // Enter directory if it's present
    directoryPathEdit->setText(QString::fromStdString(Data::getDataValue("programDirectory","project")));
}

QString DirectorySelectItem::getPath() const {
    return directoryPathEdit->text();
}
