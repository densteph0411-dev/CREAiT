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

void DirectorySelectItem::clickedDirectorySelectButton() {
    QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("Select Directory"),
        "",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

    if (!dir.isEmpty()) {
        directoryPathEdit->setText(dir); // Example
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
