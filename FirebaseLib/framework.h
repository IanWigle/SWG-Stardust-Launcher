#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <cstdio>

#include <iostream>

#include <algorithm>
#include <string>

#include <ctime>
#include <sstream>
#include <string>

#include <direct.h>
#include <assert.h>
#include <wtypes.h>

#include <locale>
#include <codecvt>

#define chdir _chdir

#define TO_STRING_EXPAND(X) #X
#define TO_STRING(X) TO_STRING_EXPAND(X)

static bool quit = false;

static BOOL WINAPI SignalHandler(DWORD event) {
    if (!(event == CTRL_C_EVENT || event == CTRL_BREAK_EVENT)) {
        return FALSE;
    }
    quit = true;
    return TRUE;
}

static bool ProcessEvents(int msec) {
    Sleep(msec);
    return quit;
}

std::string PathForResource() {
    return std::string();
}

static void ChangeToFileDirectory(const char* file_path) {
    std::string path(file_path);
    std::replace(path.begin(), path.end(), '\\', '/');
    auto slash = path.rfind('/');
    if (slash != std::string::npos) {
        std::string directory = path.substr(0, slash);
        if (!directory.empty()) chdir(directory.c_str());
    }
}

// Returns the number of microseconds since the epoch.
int64_t WinGetCurrentTimeInMicroseconds() {
    FILETIME file_time;
    GetSystemTimeAsFileTime(&file_time);

    ULARGE_INTEGER now;
    now.LowPart = file_time.dwLowDateTime;
    now.HighPart = file_time.dwHighDateTime;

    // Windows file time is expressed in 100s of nanoseconds.
    // To convert to microseconds, multiply x10.
    return now.QuadPart * 10LL;
}