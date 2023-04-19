#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>

#include <iostream>

#include <algorithm>
#include <string>

#include <ctime>
#include <sstream>
#include <string>

#include <direct.h>
#include <assert.h>
#include <wtypes.h>

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