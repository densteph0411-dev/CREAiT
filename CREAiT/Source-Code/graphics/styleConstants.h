#ifndef STYLECSONSTANTS_H
#define STYLECSONSTANTS_H


#include "qsize.h"
enum MainPageID {
    PROJECTS,
    DOCUMENTS,
    REVIEWS,
    AUTOMATIONS,
    PROJECT_SETTINGS
};
// --- Main Window Constants ---
constexpr int MainWindowNormalWidth = 1366;
constexpr int MainWindowNormalHeight = 768;

constexpr int MainWindowLeftMargin = 14;
constexpr int MainWindowRightMargin = 14;
constexpr int MainWindowTopMargin = 12;
constexpr int MainWindowBottomMargin = 12;
constexpr int MainWindowSpacing = 14;

constexpr int MainWindowHeaderToMainPanelSpacing = 12;


// --- SideBar Constants ---
constexpr int SideBarWidth = 102;
constexpr int SieBarHeight = -1;

constexpr int SidebarSpacing = 10;
constexpr int SidebarTopAddSpacing = 58;
constexpr int SidebarLogoToButtonGroupSpacing = 26;

constexpr int SideBarLogoWidth = 96;
constexpr int SideBarLogoHeight = 60;

// --- Header Constants ---
constexpr int HeaderWidth = -1;
constexpr int HeaderHeight = 42;


constexpr int AddressBarItemWidth = -1;
constexpr int AddressBarItemHeight = 42;
constexpr int AddressBarItemSpacing = 8;
constexpr int AddressBarItemMarginLeft = 16;
constexpr int AddressBarItemMarginRight = 16;
constexpr int AddressBarItemIconSize = 20;

// --- Window Control Bar Constants ---
constexpr int WindowControlBarWidth = -1;
constexpr int WindowControlBarHeight = -1;
constexpr int WindowControlBarSpacing = 18;

constexpr int WindowControlBarButtonWidth = 32;
constexpr int WindowControlBarButtonHeight = 32;

constexpr QSize WindowControlBarFunctionIconSize(32, 32);
constexpr QSize WindowControlBarControlIconSize(24, 24);

#endif // STYLECSONSTANTS_H
