#include <QThread>
#include <QCheckBox>
#include <QPointer>
#include "testWindow.h"
#include "ui_testWindow.h"
#include <integrationtest.h>
#include "IntegrationTestWorker.h"

TestWindow::TestWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    // Now you can safely access any of the elements like:
    // ui->runAllIntegrationTestsButton->setText("Go!");
}

TestWindow::~TestWindow()
{
    delete ui;
}

void TestWindow::on_runAllIntegrationTestsButton_clicked()
{
    // Optional: guard against re-entry
    this->ui->runAllIntegrationTestsButton->setEnabled(false);
    this->setCursor(Qt::BusyCursor);

    // Create worker + thread
    QThread *thread = new QThread(this);
    IntegrationTestWorker *worker = new IntegrationTestWorker();

    worker->moveToThread(thread);

    // Start worker when thread starts
    connect(thread, &QThread::started, worker, &IntegrationTestWorker::run);

    // Update UI as each test completes (queued connection across threads)
    connect(worker, &IntegrationTestWorker::testFinished, this,
            [this](const QString &checkboxObjectName, bool passed)
            {
                // Find the checkbox by its objectName and update it
                if (QCheckBox *cb = this->findChild<QCheckBox*>(checkboxObjectName)) {
                    cb->setChecked(passed);
                    cb->update();
                }
            });

    // Clean up and restore UI at the end
    connect(worker, &IntegrationTestWorker::allTestsFinished, thread, &QThread::quit);
    connect(worker, &IntegrationTestWorker::allTestsFinished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    connect(thread, &QThread::finished, this, [this]() {
        this->unsetCursor();
        this->ui->runAllIntegrationTestsButton->setEnabled(true);
    });

    thread->start();
}

