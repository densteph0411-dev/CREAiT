#ifndef APPSCALE_H
#define APPSCALE_H
// AppScale.h — compute app-scale from design vs screen & Windows scale; snap to fixed levels
#pragma once

#include <windows.h>
#include <array>
#include <algorithm>
#include <cmath>
#include "windows_scale.h"

// Struct to return multiple values from scale computation
struct AppScaleInfo
{
    double rawFactor;      // unsnapped factor computed from design vs screen
    double snappedFactor;  // factor snapped to one of AllowedPercents()
    int    screenW;        // primary screen width in physical pixels
    int    screenH;        // primary screen height in physical pixels
};

// Your allowed zoom levels (percent)
inline const std::array<int,8>& AllowedPercents() {
    static const std::array<int,8> k = {75, 100, 125, 150, 175, 200, 225, 250};
    return k;
}

// Compute unsnapped factor using design vs screen and Windows "Scale"
inline double ComputeRawAppScaleFromDesign(int designW,
                                           int designH,
                                           int &screenWOut,
                                           int &screenHOut)
{
    const double winScale = getWindowsScaleFactorPrimary(); // e.g., 2.5 for 250%
    int screenW = GetSystemMetrics(SM_CXSCREEN);            // physical px (primary)
    int screenH = GetSystemMetrics(SM_CYSCREEN);

    screenWOut = screenW;
    screenHOut = screenH;

    if (screenW <= 0 || screenH <= 0 || winScale <= 0.0) return 1.0;

    // ---- Ultra-wide detection ----
    // aspect ~ 1.78 = 16:9, 2.37 ~ 2.4 = 21:9 ultra-wide, 3.2 ~ 3.6 = 32:9 super ultra-wide
    const double aspect = double(screenW) / double(screenH);
    if (aspect > 2.3 && aspect < 3.2) {
        // ultra-wide monitors
    } else if (aspect >= 3.2) {
        // super ultra-wide monitors
        screenW /= 2;
    }

    // Fit by the smaller dimension so UI fits entirely
    const double sW = (double(screenW) / winScale) / double(designW);
    double appScale = sW;

    // keep within sane bounds
    if (appScale < 0.5) appScale = 0.5;
    if (appScale > 3.0) appScale = 3.0;
    return appScale;
}

// Snap a factor (e.g., 1.17) to nearest allowed percent (returns snapped factor)
inline double SnapFactorToAllowed(double factor) {
    const int pct = (int)std::lround(factor * 100.0);
    const auto& levels = AllowedPercents();
    int best = levels.front();
    int bestDelta = std::abs(best - pct);
    for (int v : levels) {
        // int d = std::abs(v - pct);
        // if (d < bestDelta) { bestDelta = d; best = v; }
        if (v <= pct) {
            best = v;
        }
    }
    return best / 100.0; // back to factor
}

// One-shot: compute and return full info (raw factor + snapped factor + screen size)
inline AppScaleInfo ComputeSnappedAppScale(int designW = 1920, int designH = 1080)
{
    AppScaleInfo info{};
    int screenW = 0;
    int screenH = 0;

    info.rawFactor     = ComputeRawAppScaleFromDesign(designW, designH, screenW, screenH);
    info.snappedFactor = SnapFactorToAllowed(info.rawFactor);
    info.screenW       = screenW;
    info.screenH       = screenH;

    return info;
}

#endif // APPSCALE_H
