#include "dirselectedit.h"
#include <QFileDialog>

DirSelectEdit::DirSelectEdit(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    connect(dirSelectButton, &QPushButton::clicked, this, &DirSelectEdit::clickedDirSelectButton);
}

DirSelectEdit::~DirSelectEdit()
{

}

void DirSelectEdit::clickedDirSelectButton() {
    QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("Select Directory"),
        "",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

    if (!dir.isEmpty()) {
        lineEdit->setText(dir); // Example
    }
}
