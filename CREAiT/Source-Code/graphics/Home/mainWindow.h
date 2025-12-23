#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QMouseEvent>
#include <QShowEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSystemTrayIcon>
#include <QMap>
#include <QAbstractButton>
#include <QLineEdit>
#include <QComboBox>

#include <styleConstants.h>
#include <styleSheet.h>
#include "ui_mainWindow.h"
#include <sidebar.h>
#include <header.h>
#include "aichatbotpopup.h"

enum ResizeRegion {
    None,
    Top,
    Bottom,
    Left,
    Right,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
};


class MainWindow : public QMainWindow, public Ui::MainWindow, public StyleSheet {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void initializeWidgets();
    void initializeTrayIcon();
    void loadFonts();
    void navigateToPage(int pageIndex);

    ResizeRegion detectResizeRegion(const QPoint &pos);
    void setCursorForRegion(ResizeRegion region);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

signals:
    void requestedButtonSelect(int buttonIndex);

private slots:
    void activatedTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void showPage(int pageIndex);
    void viewProject(const QString& projectsPath, const QString& projectName);
    void processAIChat();
    void showUser();
    void showZoom();
    void showAIChatPanel();
    void quit();
    void navigateTabs(int index);
    void navigateForHeader(int index, const QString& name);
    void closeAddresBarItem(int index, const QString& name);
    void openLastProject();

private:
    QMap<QString, ProjectPage*> projectPageViews;
    QMap<QString, QString> projectPaths;
    QMap<QString, int> projectSectionIndexes;
    QString currentProjectName;
    int documentsPageIndex;
    ResizeRegion resizeRegion;
    QPoint dragPosition;
    QPoint startMousePos;
    QRect startGeometry;
    QSystemTrayIcon* trayIcon;
    bool isResizing;
    bool canDragWindow;

    bool isDraggableArea(const QPoint &pos) const;
    bool isWithinRegion(QWidget *region, const QPoint &pos) const;
    void setGlobalSection(int sectionIndex);
    void switchToProjectSection(const QString& projectName, int sectionIndex, ProjectPage* projectPage = nullptr);

    AIChatBotPopup *chatBotPopup;
};
#endif // MAINWINDOW_H
