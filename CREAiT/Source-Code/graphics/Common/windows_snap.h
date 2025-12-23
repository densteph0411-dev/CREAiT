#ifndef WINDOWS_SNAP_H
#define WINDOWS_SNAP_H

#include <windows.h>

#ifndef __MINGW32__
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#else
// MinGW: include dwmapi.h (library linked via .pro file)
#include <dwmapi.h>
#endif

// Windows 11 DWM attributes for snap layouts
#ifndef DWMWA_WINDOW_CORNER_PREFERENCE
#define DWMWA_WINDOW_CORNER_PREFERENCE 33
#endif

#ifndef DWMWA_BORDER_COLOR
#define DWMWA_BORDER_COLOR 34
#endif

#ifndef DWMWA_CAPTION_COLOR
#define DWMWA_CAPTION_COLOR 35
#endif

#ifndef DWMWA_TEXT_COLOR
#define DWMWA_TEXT_COLOR 36
#endif

#ifndef DWMWA_SYSTEMBACKDROP_TYPE
#define DWMWA_SYSTEMBACKDROP_TYPE 38
#endif

// Enable Windows 11 snap layouts for a frameless window
inline bool enableWindows11SnapLayouts(HWND hwnd) {
    if (!hwnd)
        return false;

    // Check if running on Windows 11 (build 22000 or later)
    OSVERSIONINFOEXW osvi = { sizeof(osvi) };
    DWORDLONG dwlConditionMask = 0;
    VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
    VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);
    VER_SET_CONDITION(dwlConditionMask, VER_BUILDNUMBER, VER_GREATER_EQUAL);
    
    osvi.dwMajorVersion = 10;
    osvi.dwMinorVersion = 0;
    osvi.dwBuildNumber = 22000; // Windows 11 build number

    if (!VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER, dwlConditionMask)) {
        // Not Windows 11, snap layouts not available
        return false;
    }

    // The key to enabling snap layouts is to ensure the window is recognized
    // as a standard window by Windows. For frameless windows, we need to
    // set appropriate DWM attributes.
    
    // Set window corner preference (rounded corners - Windows 11 style)
    DWORD cornerPreference = 2; // DWMWCP_ROUND (2) - enables modern window features
    HRESULT hr = DwmSetWindowAttribute(
        hwnd,
        DWMWA_WINDOW_CORNER_PREFERENCE,
        &cornerPreference,
        sizeof(cornerPreference)
    );

    // Note: Snap layouts work when Windows recognizes the window as a standard
    // application window. The frameless hint removes the title bar, but Windows
    // 11 can still provide snap layouts if the window has proper attributes.
    // However, full snap layout support may require keeping some native window
    // frame elements or using a hybrid approach.

    return SUCCEEDED(hr);
}

#endif // WINDOWS_SNAP_H

