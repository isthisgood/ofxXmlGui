/**
 *  WinScreenTopology.cpp
 *
 *  Created by Marek Bereza on 09/07/2013.
 */

#include "ScreenTopology.h"
// had to hack oF to change _WIN32_WINNT 0x0400 to 0x0500
// it's in ofConstants.h
//#   define _WIN32_WINNT 0x0500
#include <windows.h>


int ScreenTopology::getNumScreens() {
	return GetSystemMetrics (SM_CMONITORS);

}

BOOL CALLBACK monitorEnumFunc(
    HMONITOR hMonitor,
    HDC hdcMonitor,
    LPRECT lprcMonitor,
    LPARAM dwData
) {
    vector<ofRectangle> *monitors = (vector<ofRectangle> *)dwData;

    MONITORINFO monitor;
    monitor.cbSize = sizeof(MONITORINFO);

    if(GetMonitorInfo(hMonitor, &monitor)) {




        ofRectangle r(monitor.rcMonitor.left,
        monitor.rcMonitor.top,
        monitor.rcMonitor.right - monitor.rcMonitor.left,
        monitor.rcMonitor.bottom - monitor.rcMonitor.top);
        printf("%.2f  %.2f  %.2f  %.2f\n", r.x, r.y, r.width, r.height);
        monitors->push_back(r);
    } else {
        printf("failed to get info on monitor\n");
    }
    return TRUE;
}



ofRectangle ScreenTopology::getScreenRect(int index) {
	int numMonitors = GetSystemMetrics (SM_CMONITORS);
    printf("we have %d monitors\n", numMonitors);
    vector<ofRectangle> monitors;
    if(EnumDisplayMonitors(NULL, NULL, monitorEnumFunc, (LPARAM)&monitors)) {
		return monitors[index];
    }
	return ofRectangle();
}



