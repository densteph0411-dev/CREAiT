#include "documentspage.h"
#include "documentcard.h"
#include "adddocumentdialog.h"
#include <DocumentRecord.h>
#include <iostream>
#include <records.h>

// Optional but recommended explicit includes
#include <QDir>
#include <QFile>
#include <QStringList>

DocumentsPage::DocumentsPage(QWidget *parent)
    : QWidget(parent), needsRefresh(true)   // no hard-coded "documents" here
{
    setupUi(this);

    // 1) Pull the directory path from your Data store
    //    Data::getData(...) returns std::string per your note
    const std::string dir = Data::getDataValue("documentDirectory", "project");

    // 2) Convert to QString and store
    documentsPath = QString::fromStdString(dir);

    // 3) Make sure the folder (and any parents) exist
    //    mkpath creates intermediate folders safely
    if (!documentsPath.isEmpty())
        QDir().mkpath(documentsPath);

    // 4) Layout and wiring as before
    layout = new FlowLayout(container, 20, 24, 24);
    container->setLayout(layout);

    connect(uploadButton, &QPushButton::clicked,
            this, &DocumentsPage::uploadButtonClicked);
}

DocumentsPage::~DocumentsPage() { }

void DocumentsPage::addLayout(const QFileInfo& fileInfo) {
    DocumentCard* card = new DocumentCard(fileInfo);
    layout->addWidget(card);

    connect(card, &DocumentCard::requestRefresh, this, &DocumentsPage::refresh);
}

void DocumentsPage::uploadButtonClicked() {
    QStringList filePaths = QFileDialog::getOpenFileNames(this, tr("Select Documents"));
    if (filePaths.isEmpty())
        return;

    for (const QString &filePath : std::as_const(filePaths)) {
        QFileInfo fileInfo(filePath);

        // Show the Add Document dialog to collect metadata
        AddDocumentDialog* dialog = new AddDocumentDialog(this);
        
        // Pre-fill document name with the file name (without extension)
        QString defaultName = fileInfo.baseName();
        dialog->documentNameEdit->setText(defaultName);
        
        if (dialog->exec() == QDialog::Accepted) {
            QString documentName = dialog->getDocumentName();
            QString purpose = dialog->getPurpose();
            QString description = dialog->getDescription();
            
            // Build the destination path safely using QDir
            const QString destPath = QDir(documentsPath).filePath(fileInfo.fileName());

            if (!QFile::exists(destPath)) {
                QFile::copy(filePath, destPath);
                
                // Create document record with metadata
                DocumentRecord docRecord;
                std::string recordID = docRecord.create(
                    documentName.toStdString(),
                    purpose.toStdString(),
                    description.toStdString(),
                    ""  // agentID - empty for now
                );
                
                // Associate the file with the record ID if needed
                // You may want to store the recordID with the file somehow
            }

            addLayout(QFileInfo(destPath));
        }
        
        delete dialog;
    }
    needsRefresh = false;
}

void DocumentsPage::refresh() {
    // Update documents path from Data dictionary (in case project changed)
    const std::string dirValue = Data::getDataValue("documentDirectory", "project");
    if (!dirValue.empty()) {
        QString newPath = QString::fromStdString(dirValue);
        if (documentsPath != newPath) {
            documentsPath = newPath;
            QDir().mkpath(documentsPath);
        }
    }

    if (documentsPath.isEmpty()) {
        needsRefresh = true;
        return;
    }

    QDir dir(documentsPath);
    QFileInfoList oldFileList;
    QFileInfoList newFileList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    // Delete files that do not exist in the new list
    for (int i = layout->count()-1; i >= 0; --i) {
        DocumentCard* card = reinterpret_cast<DocumentCard*>(layout->itemAt(i)->widget());
        oldFileList << card->fileInfo();

        if (!newFileList.contains(card->fileInfo())) {
            layout->removeWidget(card);
            delete card;
        }
    }

    // Add files that are new
    for (const QFileInfo& fileInfo : std::as_const(newFileList)) {
        if (!oldFileList.contains(fileInfo)) {
            addLayout(fileInfo);
        }
    }

    needsRefresh = false;
}

void DocumentsPage::markDirty() {
    needsRefresh = true;
}

void DocumentsPage::ensureRefreshed() {
    if (needsRefresh) {
        refresh();
    }
}
