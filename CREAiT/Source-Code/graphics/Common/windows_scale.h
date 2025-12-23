#ifndef WINDOWS_SCALE_H
#define WINDOWS_SCALE_H

#include <windows.h>

#ifndef __MINGW32__
#include <shellscalingapi.h>
#pragma comment(lib, "Shcore.lib")
#else
typedef enum PROCESS_DPI_AWARENESS {
    PROCESS_DPI_UNAWARE = 0,
    PROCESS_SYSTEM_DPI_AWARE = 1,
    PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

typedef enum MONITOR_DPI_TYPE {
    MDT_EFFECTIVE_DPI = 0,
    MDT_ANGULAR_DPI = 1,
    MDT_RAW_DPI = 2,
    MDT_DEFAULT = MDT_EFFECTIVE_DPI
} MONITOR_DPI_TYPE;

extern "C" {
    HRESULT WINAPI SetProcessDpiAwareness(PROCESS_DPI_AWARENESS value);
    HRESULT WINAPI GetDpiForMonitor(HMONITOR hmonitor, MONITOR_DPI_TYPE dpiType, UINT *dpiX, UINT *dpiY);
}
#endif

#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Shcore.lib")

inline bool makePerMonitorDpiAware() {
    // Try the newest API first (Win10+)
    using SetCtxFn = BOOL (WINAPI*)(DPI_AWARENESS_CONTEXT);
    if (auto p = reinterpret_cast<SetCtxFn>(GetProcAddress(GetModuleHandleW(L"user32"), "SetProcessDpiAwarenessContext"))) {
        if (p(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) return true;
        p(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
        return true;
    }
    // Fallback (Win 8.1): Shcore SetProcessDpiAwareness
    if (SUCCEEDED(SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE))) return true;
    // Old fallback
    SetProcessDPIAware();
    return false;
}

inline double getWindowsScaleFactorPrimary() {
    // Ensure the process is DPI aware so we get real values
    makePerMonitorDpiAware();

    POINT pt = { 0, 0 };
    HMONITOR hm = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
    UINT dpiX = 96, dpiY = 96;
    if (SUCCEEDED(GetDpiForMonitor(hm, MDT_EFFECTIVE_DPI, &dpiX, &dpiY))) {
        return static_cast<double>(dpiX) / 96.0;   // 1.0 = 100%, 2.5 = 250%
    }

    // Fallbacks if API missing
    if (auto pGetDpiForSystem = reinterpret_cast<UINT(WINAPI*)()>(
            GetProcAddress(GetModuleHandleW(L"user32"), "GetDpiForSystem"))) {
        return static_cast<double>(pGetDpiForSystem()) / 96.0;
    }

    HDC hdc = GetDC(nullptr);
    int dpi = GetDeviceCaps(hdc, LOGPIXELSX);
    ReleaseDC(nullptr, hdc);
    return static_cast<double>(dpi) / 96.0;
}

#endif // WINDOWS_SCALE_H
