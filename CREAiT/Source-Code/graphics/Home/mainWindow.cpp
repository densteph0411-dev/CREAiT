#include <QApplication>
#include <QElapsedTimer>
#include <QFontDatabase>
#include <QMenu>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

#include "mainWindow.h"
#include <userinfodialog.h>
#include <Presenter/projectdatapresenter.h>
#include <loadingdialog.h>
#include "aichatbotpopup.h"
#include "recordsummarypopover.h"
#include "zoomsettingdialog.h"
#include <records.h>
#include "projectFileOperations.h"
#include <QAbstractButton>
#include <QLineEdit>
#include <QComboBox>
#include <documentspage.h>
#include <windows_snap.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      documentsPageIndex(-1),
      resizeRegion(None),
      startMousePos(-1, -1),
      isResizing(false),
      canDragWindow(false),
      dragPosition(-1, -1)
{
    initializeWidgets();
    navigateTabs(0);

    RecordSummaryPopover::instance(this);  // this = main window
}

MainWindow::~MainWindow() {

}

void MainWindow::initializeWidgets() {
    setupUi(this);

    initializeTrayIcon();

    loadFonts();

    connect(sideBar, &SideBar::requestedPage, this, &MainWindow::showPage);

    connect(header, &Header::handleUser, this, &MainWindow::showUser);
    connect(header, &Header::handleZoom, this, &MainWindow::showZoom);
    connect(header, &Header::handleAIChat, this, &MainWindow::showAIChatPanel);
    connect(header, &Header::handleTabNavigation, this, &MainWindow::navigateForHeader);
    connect(header, &Header::handleAddressBarItemClose, this, &MainWindow::closeAddresBarItem);
    connect(homePage, &HomePage::requestLoadProject, this, &MainWindow::navigateTabs);
    connect(homePage, &HomePage::requestCreateProject, this, &MainWindow::navigateTabs);
    connect(projectsViewPage, &ProjectsViewPage::requestCreateProject, this, &MainWindow::navigateTabs);
    connect(projectsViewPage, &ProjectsViewPage::requestViewProject, this, &MainWindow::viewProject);
    connect(createProjectPage, &CreateProjectPage::requestBackPage, this, &MainWindow::navigateTabs);
    connect(createProjectPage, &CreateProjectPage::projectCreated, this, &MainWindow::viewProject);

    connect(this, &MainWindow::requestedButtonSelect, sideBar, &SideBar::selectedButton);

    chatBotPopup = new AIChatBotPopup(this);
    chatBotPopup->hide();

    // this->mainPanel->setStyleSheet("background-color: white; border-top-left-radius: 16px; border-top-right-radius: 16px;");
    this->setStyleSheet(loadStyleSheet(":/stylesheets/Home/MainWindow.qss"));

    QTimer::singleShot(3000, this, &MainWindow::openLastProject);
}

void MainWindow::openLastProject() {
    std::string projectsPath = Data::getDataValue("programDirectory", "project");
    fileOperations::parseProjectFile parseProjectFile(projectsPath);
    std::string activeProjectName = parseProjectFile.activeProjectName;
    if (projectsPath != "" && activeProjectName != "")
        viewProject(QString::fromStdString(projectsPath), QString::fromStdString(activeProjectName));

    if (documentsPage) {
        documentsPageIndex = mainPanel->indexOf(documentsPage);
        documentsPage->markDirty();
    }
}

void MainWindow::initializeTrayIcon() {
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":images/app_icon.ico"));
    trayIcon->setToolTip("CREAiT v1.0");

    // Create context menu
    QMenu *trayMenu = new QMenu(this);

    QAction *showAction = new QAction("Show", this);
    QAction *exitAction = new QAction("Exit", this);

    trayMenu->addAction(showAction);
    trayMenu->addSeparator();
    trayMenu->addAction(exitAction);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    // Connect actions
    connect(showAction, &QAction::triggered, this, &QWidget::show);
    connect(exitAction, &QAction::triggered, this, &MainWindow::quit);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::activatedTrayIcon);
}

void MainWindow::loadFonts() {
    auto loadFontIfLoaded = [this](const QString &fontPath, int pointSize) {
        int fontId = QFontDatabase::addApplicationFont(fontPath);
        if (fontId != -1) {
            QStringList families = QFontDatabase::applicationFontFamilies(fontId);
            if (!families.isEmpty()) {
                QFont font(families.at(0), pointSize);
                this->setFont(font); // Apply globally to this QMainWindow
            } else {
                qDebug() << "Font loaded but no families found:" << fontPath;
            }
        } else {
            qDebug() << "Failed to load font:" << fontPath;
        }
    };

    loadFontIfLoaded(":/fonts/Open_Sans/static/OpenSans-Regular.ttf", 14);
    loadFontIfLoaded(":/fonts/Open_Sans/static/OpenSans-Bold.ttf", 14);
    loadFontIfLoaded(":/fonts/Open_Sans/static/OpenSans-Medium.ttf", 14);
    loadFontIfLoaded(":/fonts/Open_Sans/static/OpenSans-Light.ttf", 14);
    loadFontIfLoaded(":/fonts/Open_Sans/static/OpenSans-SemiBold.ttf", 14);
}

// Main panel navigation
void MainWindow::navigateToPage(int pageIndex) {
    if (pageIndex < 0 || pageIndex >= mainPanel->count()) {
        qDebug() << "Invalid page index:" << pageIndex;
        return;
    }

    emit requestedButtonSelect(pageIndex);
    mainPanel->setCurrentIndex(pageIndex);
}

void MainWindow::navigateTabs(int index) {
    QObject *s = sender();  // who emitted the signal?

    if (index == 2 && s == homePage) {
        createProjectPage->setSource(CreateProjectPage::Source::HomePage);
    } else if (index == 2 && s == projectsViewPage) {
        createProjectPage->setSource(CreateProjectPage::Source::ProjectsViewPage);
    }
    if(index < 3) {
        stackedWidget->setCurrentIndex(index);
        header->windowControlBar->aiChatButton->hide();
        sideBar->automationsButton->hide();
        sideBar->projectsButton->hide();
        sideBar->documentsButton->hide();
        sideBar->reviewsButton->hide();
        sideBar->projectSettingsButton->hide();
    } else {
        stackedWidget->setCurrentIndex(index);
        header->windowControlBar->aiChatButton->show();
        if (projectInfoRecord::AutomationsEnabled) {
            sideBar->automationsButton->show();
        }
        sideBar->projectsButton->show();
        if (projectInfoRecord::DocumentsEnabled) {
            sideBar->documentsButton->show();
        }
        if (projectInfoRecord::ReviewsEnabled) {
            sideBar->reviewsButton->show();
        }
        sideBar->projectSettingsButton->show();
    }
}

void MainWindow::navigateForHeader(int index, const QString& name) {
    if (index == 0) {
        currentProjectName.clear();
        stackedWidget->setCurrentIndex(1);
        header->windowControlBar->aiChatButton->hide();
        sideBar->automationsButton->hide();
        sideBar->projectsButton->hide();
        sideBar->documentsButton->hide();
        sideBar->reviewsButton->hide();
        sideBar->projectSettingsButton->hide();
        setGlobalSection(PROJECTS);
    } else {
        if (projectPaths.contains(name)) {
            viewProject(projectPaths.value(name), name);
        } else if (projectPageViews.contains(name)) {
            viewProject(ProjectDataPresenter::instance().getProjectPath(), name);
        }
    }
}

void MainWindow::closeAddresBarItem(int index, const QString& name) {
    if (projectPageViews.contains(name)) {
        QWidget *page = projectPageViews.take(name);
        if (page) {
            mainPanel->removeWidget(page);
            page->deleteLater();
        }
        projectPaths.remove(name);
        projectSectionIndexes.remove(name);
        if (currentProjectName == name)
            currentProjectName.clear();

        if (projectPageViews.size() < 1) {
            navigateForHeader(0, name);
        } else {
            auto it = std::prev(projectPageViews.constEnd());
            QString projectName = it.key();
            navigateForHeader(1, projectName);
        }
    }
}

void MainWindow::activatedTrayIcon(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick) {
        this->showNormal();
        this->raise();
        this->activateWindow();
    }
}

void MainWindow::showPage(int pageIndex) {
    if (stackedWidget->currentIndex() == 3 && !currentProjectName.isEmpty()) {
        switchToProjectSection(currentProjectName, pageIndex);
    } else {
        setGlobalSection(pageIndex);
    }
}

void MainWindow::viewProject(const QString& projectsPath, const QString& projectName) {
    if (projectPageViews.contains(projectName)) {
        ProjectPage* currentProectPage = projectPageViews.value(projectName);
        QString pathToLoad = projectsPath;
        if (projectPaths.contains(projectName))
            pathToLoad = projectPaths.value(projectName);
        else
            projectPaths.insert(projectName, projectsPath);

        LoadingDialog *loadingDialog = new LoadingDialog(this);
        loadingDialog->show();

        // Load project asynchronously
        QTimer::singleShot(0, this, [=]() {
            QFuture<void> future = QtConcurrent::run([=]() {
                ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();
                projInstance.loadProject(pathToLoad, projectName);
            });

            QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
            connect(watcher, &QFutureWatcher<void>::finished, this, [=]() {
                header->addItemtoAddressBar(projectName);
                currentProjectName = projectName;
                if (documentsPage)
                    documentsPage->markDirty();
                if (projecSettingsPage)
                    projecSettingsPage->markDirty();
                int sectionIndex = projectSectionIndexes.value(projectName, PROJECTS);
                navigateTabs(3);
                switchToProjectSection(projectName, sectionIndex, currentProectPage);

                QTimer::singleShot(500, this, [=]() {
                    loadingDialog->close();
                    loadingDialog->deleteLater();
                });

                watcher->deleteLater();
            });

            watcher->setFuture(future);
        });
    } else {
        LoadingDialog *loadingDialog = new LoadingDialog(this);
        loadingDialog->show();

        // Start the background loading in next event loop cycle
        QTimer::singleShot(0, this, [=]() {
            QFuture<void> future = QtConcurrent::run([=]() {
                ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();
                projInstance.loadProject(projectsPath, projectName);
            });

            ProjectPage* projectPage = new ProjectPage(this);
            mainPanel->addWidget(projectPage);
            projectPageViews.insert(projectName, projectPage);
            projectPaths.insert(projectName, projectsPath);

            QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
            connect(watcher, &QFutureWatcher<void>::finished, this, [=]() {
                header->addItemtoAddressBar(projectName);
                currentProjectName = projectName;
                projectSectionIndexes.insert(projectName, PROJECTS);
                if (documentsPage)
                    documentsPage->markDirty();
                navigateTabs(3);
                switchToProjectSection(projectName, PROJECTS, projectPage);

                QTimer::singleShot(500, this, [=]() {
                    loadingDialog->close();
                    loadingDialog->deleteLater();
                });

                watcher->deleteLater();
            });

            watcher->setFuture(future);
        });
    }
}

void MainWindow::processAIChat() {
    // Handle menu button click
}

void MainWindow::showUser() {
    // Handle avatar button click
    UserInfoDialog* dialog = new UserInfoDialog();
    dialog->setData();
    dialog->show();
}

void MainWindow::showZoom() {
    // Handle avatar button click
    ZoomSettingDialog* dialog = new ZoomSettingDialog();
    dialog->show();
}

void MainWindow::showAIChatPanel() {
    chatBotPopup->setFixedSize(chatBotPopup->width(), stackedWidget->height());
    QPoint topRightPos = stackedWidget->mapTo(this, stackedWidget->rect().topRight());;
    chatBotPopup->move(topRightPos - QPoint(chatBotPopup->width(), 0));
    chatBotPopup->show();
    chatBotPopup->raise();
}

void MainWindow::quit() {
    if (trayIcon) {
        trayIcon->hide();
        delete trayIcon;
        trayIcon = nullptr;
    }

    QApplication::quit();
}

ResizeRegion MainWindow::detectResizeRegion(const QPoint &pos) {
    const int margin = 10;
    bool onLeft = pos.x() <= margin;
    bool onRight = pos.x() >= width() - margin;
    bool onTop = pos.y() <= margin;
    bool onBottom = pos.y() >= height() - margin;

    if (onTop && onLeft) return TopLeft;
    if (onTop && onRight) return TopRight;
    if (onBottom && onLeft) return BottomLeft;
    if (onBottom && onRight) return BottomRight;
    if (onTop) return Top;
    if (onBottom) return Bottom;
    if (onLeft) return Left;
    if (onRight) return Right;
    return None;
}

void MainWindow::setCursorForRegion(ResizeRegion region) {
    switch (region) {
    case Top: case Bottom: setCursor(Qt::SizeVerCursor); break;
    case Left: case Right: setCursor(Qt::SizeHorCursor); break;
    case TopLeft: case BottomRight: setCursor(Qt::SizeFDiagCursor); break;
    case TopRight: case BottomLeft: setCursor(Qt::SizeBDiagCursor); break;
    default: setCursor(Qt::ArrowCursor); break;
    }
}

bool MainWindow::isWithinRegion(QWidget *region, const QPoint &pos) const {
    if (!region)
        return false;

    QPoint localPoint = region->mapFrom(this, pos);
    if (!region->rect().contains(localPoint))
        return false;

    QWidget *child = region->childAt(localPoint);
    if (child && (qobject_cast<QAbstractButton*>(child) ||
                  qobject_cast<QLineEdit*>(child) ||
                  qobject_cast<QComboBox*>(child)))
        return false;

    return true;
}

bool MainWindow::isDraggableArea(const QPoint &pos) const {
    return isWithinRegion(header, pos) ||
           isWithinRegion(sideBar, pos) ||
           isWithinRegion(logoContainer, pos);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        startMousePos = event->globalPosition().toPoint();
        startGeometry = geometry();
        resizeRegion = detectResizeRegion(event->pos());
        isResizing = (resizeRegion != None);

        canDragWindow = !isResizing && isDraggableArea(event->pos());

        if (canDragWindow) {
            dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        } else if (!isResizing) {
            dragPosition = QPoint(-1, -1);
        } else {
            setCursorForRegion(detectResizeRegion(event->pos()));
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    QPoint globalPos = event->globalPosition().toPoint();
    if (isResizing && !isMaximized()) {
        int dx = globalPos.x() - startMousePos.x();
        int dy = globalPos.y() - startMousePos.y();
        QRect newGeom = startGeometry;

        switch (resizeRegion) {
        case Right:        newGeom.setWidth(startGeometry.width() + dx); break;
        case Bottom:       newGeom.setHeight(startGeometry.height() + dy); break;
        case BottomRight:  newGeom.setSize(QSize(startGeometry.width() + dx, startGeometry.height() + dy)); break;
        case BottomLeft:   newGeom.setLeft(startGeometry.left() + dx); newGeom.setHeight(startGeometry.height() + dy); break;
        case Left:         newGeom.setLeft(startGeometry.left() + dx); break;
        case Top:          newGeom.setTop(startGeometry.top() + dy); break;
        case TopRight:     newGeom.setTop(startGeometry.top() + dy); newGeom.setRight(startGeometry.right() + dx); break;
        case TopLeft:      newGeom.setTopLeft(startGeometry.topLeft() + QPoint(dx, dy)); break;
        default: break;
        }
        setGeometry(newGeom);
        setCursorForRegion(detectResizeRegion(event->pos()));
    } else if (event->buttons() & Qt::LeftButton && !isMaximized() && canDragWindow && !dragPosition.isNull()) {
        move(globalPos - dragPosition);
    } else {
        ResizeRegion region = detectResizeRegion(event->pos());
        if (region != None) {
            setCursorForRegion(region);
        } else {
            setCursor(Qt::ArrowCursor);
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    isResizing = false;
    resizeRegion = None;
    startMousePos = QPoint(-1, -1);
    dragPosition = QPoint(-1, -1);
    canDragWindow = false;
    setCursor(Qt::ArrowCursor);
}

void MainWindow::setGlobalSection(int sectionIndex) {
    int indexToUse = sectionIndex;
    if (indexToUse < PROJECTS)
        indexToUse = PROJECTS;
    else if (indexToUse > PROJECT_SETTINGS)
        indexToUse = PROJECT_SETTINGS;

    if (indexToUse == PROJECTS) {
        mainPanel->setCurrentIndex(PROJECTS);
    } else {
        mainPanel->setCurrentIndex(indexToUse);
        // Refresh documents when Documents section is selected
        if (documentsPage && indexToUse == DOCUMENTS)
            documentsPage->ensureRefreshed();
    }

    if (sideBar)
        sideBar->selectedButton(indexToUse);
}

void MainWindow::switchToProjectSection(const QString& projectName, int sectionIndex, ProjectPage* projectPage) {
    int indexToUse = sectionIndex;
    if (indexToUse < PROJECTS)
        indexToUse = PROJECTS;
    else if (indexToUse > PROJECT_SETTINGS)
        indexToUse = PROJECT_SETTINGS;

    if (indexToUse == PROJECTS) {
        ProjectPage* page = projectPage ? projectPage : projectPageViews.value(projectName, nullptr);
        if (page)
            mainPanel->setCurrentWidget(page);
        else
            mainPanel->setCurrentIndex(PROJECTS);
    } else {
        mainPanel->setCurrentIndex(indexToUse);
        // Refresh documents when Documents section is selected (ensures project-specific documents are shown)
        if (documentsPage && indexToUse == DOCUMENTS)
            documentsPage->ensureRefreshed();
        if (projecSettingsPage && indexToUse == PROJECT_SETTINGS)
            projecSettingsPage->ensureRefreshed();
    }

    projectSectionIndexes[projectName] = indexToUse;
    if (sideBar)
        sideBar->selectedButton(indexToUse);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton)
        return;

    // Check if the Y coordinate is within the top 100 pixels
    if (event->pos().y() > 100)
        return;

    // Check if the point is inside any child widget (except the MainWindow itself)
    QWidget *childAtPos = childAt(event->pos());
    // if (childAtPos && childAtPos != this)
    //     return;
    if (childAtPos && qobject_cast<QAbstractButton*>(childAtPos))
        return;
}

void MainWindow::leaveEvent(QEvent *event) {
    Q_UNUSED(event);
    if (!isResizing)
        setCursor(Qt::ArrowCursor);
}


void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event); // always call base class version first

    if (chatBotPopup->isVisible()) {
        chatBotPopup->setFixedSize(chatBotPopup->width(), stackedWidget->height());
        QPoint topRightPos = stackedWidget->mapTo(this, stackedWidget->rect().topRight());;
        chatBotPopup->move(topRightPos - QPoint(chatBotPopup->width(), 0));
    }
}

void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);
    
    // Enable Windows 11 snap layouts when window is shown
    // We need to wait until the window is shown to get a valid HWND
    HWND hwnd = reinterpret_cast<HWND>(winId());
    if (hwnd) {
        enableWindows11SnapLayouts(hwnd);
    }
}
