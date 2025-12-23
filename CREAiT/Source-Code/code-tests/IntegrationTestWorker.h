#pragma once
#include <QObject>
#include "integrationTest.h"   // your test class header

class IntegrationTestWorker : public QObject {
    Q_OBJECT
public:
    explicit IntegrationTestWorker(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void testFinished(const QString &checkboxObjectName, bool passed);
    void allTestsFinished();

public slots:
    void run() {
        // Run strictly in order — one after another
        // Project Integration Tests
        emit testFinished(QStringLiteral("createProjectTestCheckBox"),        testObj.createProjectTest());
        emit testFinished(QStringLiteral("closeProjectTestCheckBox"),         testObj.closeProjectTest());
        emit testFinished(QStringLiteral("openProjectTestCheckBox"),          testObj.openProjectTest());
        emit testFinished(QStringLiteral("createManyProjectTestsCheckBox"),   testObj.createManyProjectTests());
        emit testFinished(QStringLiteral("getRecentProjectsTestCheckBox"),    testObj.getRecentProjectsTest());
        emit testFinished(QStringLiteral("changeToAllProjectsTestCheckBox"),  testObj.changeToAllProjectsTest());

        // Record Integration Tests
        emit testFinished(QStringLiteral("createRecordTestCheckBox"),         testObj.createRecordTest());
        emit testFinished(QStringLiteral("createManyRecordsTestCheckBox"),    testObj.createManyRecordsTest());
        emit testFinished(QStringLiteral("removeRecordsTestCheckBox"),        testObj.removeRecordsTest());
        emit testFinished(QStringLiteral("addLinksToRecordTestCheckBox"),     testObj.addLinksToRecordTest());
        emit testFinished(QStringLiteral("removeLinksToRecordTestCheckBox"),  testObj.removeLinksToRecordTest());
        emit testFinished(QStringLiteral("createManyLinksTest"),              testObj.createManyLinksTest());

        // Automation test
        emit testFinished(QStringLiteral("automationLifeCycleTest"),          testObj.createManyAutomationsTest());

        // Record Type test
        emit testFinished(QStringLiteral("getImagePathTest"),                 testObj.testRecordTypePaths());
        emit testFinished(QStringLiteral("connectionTest"),                   testObj.createIntegrations());
        emit testFinished(QStringLiteral("userTest"),                         testObj.createUserData());

        emit allTestsFinished();
    }

private:
    integrationTest testObj;   // persists across steps
};
