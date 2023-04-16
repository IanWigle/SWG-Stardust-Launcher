#pragma once
// Desktop Libraries
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>

#include <iostream>

#include <algorithm>
#include <string>

#include <ctime>
#include <sstream>
#include <string>

#pragma warning ( disable : 4099 )
#pragma warning ( disable : 4081 )

static bool quit = false;

// Windows related libraries and functions
#ifdef _WIN32
#include <direct.h>
#include <windows.h>

#define chdir _chdir

static BOOL WINAPI SignalHandler(DWORD event) {
    if (!(event == CTRL_C_EVENT || event == CTRL_BREAK_EVENT)) {
        return FALSE;
    }
    quit = true;
    return TRUE;
}
#else
#include <unistd.h>

static void SignalHandler(int /* ignored */) { quit = true; }
#endif  // _WIN32

#define TO_STRING_EXPAND(X) #X
#define TO_STRING(X) TO_STRING_EXPAND(X)

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

#if defined(_WIN32)
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
#endif

// Firebase headers
#include "firebase/app.h"
#include "firebase/auth.h"
#include "firebase/auth/credential.h"
#include "firebase/util.h"

using ::firebase::App;
using ::firebase::AppOptions;
using ::firebase::Future;
using ::firebase::FutureBase;
using ::firebase::Variant;
using ::firebase::auth::AdditionalUserInfo;
using ::firebase::auth::Auth;
using ::firebase::auth::AuthError;
using ::firebase::auth::Credential;
using ::firebase::auth::EmailAuthProvider;
using ::firebase::auth::FacebookAuthProvider;
using ::firebase::auth::GitHubAuthProvider;
using ::firebase::auth::GoogleAuthProvider;
using ::firebase::auth::kAuthErrorFailure;
using ::firebase::auth::kAuthErrorInvalidCredential;
using ::firebase::auth::kAuthErrorInvalidProviderId;
using ::firebase::auth::kAuthErrorNone;
using ::firebase::auth::OAuthProvider;
using ::firebase::auth::PhoneAuthProvider;
using ::firebase::auth::PlayGamesAuthProvider;
using ::firebase::auth::SignInResult;
using ::firebase::auth::TwitterAuthProvider;
using ::firebase::auth::User;
using ::firebase::auth::UserInfoInterface;
using ::firebase::auth::UserMetadata;

// Set this to true, and set the email/password, to test a custom email address.
static const bool kTestCustomEmail = false;
static const char kCustomEmail[] = "custom.email@example.com";
static const char kCustomPassword[] = "CustomPasswordGoesHere";

// Constants used during tests.
static const char kTestNonceBad[] = "testBadNonce";
static const char kTestPassword[] = "testEmailPassword123";
static const char kTestEmailBad[] = "bad.test.email@example.com";
static const char kTestPasswordBad[] = "badTestPassword";
static const char kTestIdTokenBad[] = "bad id token for testing";
static const char kTestAccessTokenBad[] = "bad access token for testing";
static const char kTestPasswordUpdated[] = "testpasswordupdated";
static const char kTestIdProviderIdBad[] = "bad provider id for testing";
static const char kTestServerAuthCodeBad[] = "bad server auth code";

static const int kWaitIntervalMs = 300;
static const int kPhoneAuthCodeSendWaitMs = 600000;
static const int kPhoneAuthCompletionWaitMs = 8000;
static const int kPhoneAuthTimeoutMs = 0;

static const char kFirebaseProviderId[] =
#if defined(__ANDROID__)
"firebase";
#else   // !defined(__ANDROID__)
"Firebase";
#endif  // !defined(__ANDROID__)

//#define FIREBASE_CONFIG "C:\.Development\Projects\C++\Firebase\Auth Tests\Output\Debug"

// Path to the Firebase config file to load.
#ifdef FIREBASE_CONFIG
#define FIREBASE_CONFIG_STRING TO_STRING(FIREBASE_CONFIG)
#else
#define FIREBASE_CONFIG_STRING ""
#endif  // FIREBASE_CONFIG

static void LogMessage(const char* format, ...)
{
    va_list list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
    printf("\n");
    fflush(stdout);
    //std::cout << format << std::endl;
}

#include "Logging.h"
#include "WaitFunctions.h"
#include "ExpectFunctions.h"

// Create an email that will be different from previous runs.
// Useful for testing creating new accounts.
static std::string CreateNewEmail() {
    std::stringstream email;
    email << "random_" << std::time(0) << "@gmail.com";
    return email.str();
}

#include "AuthStateChangeCounter.h"
#include "IdTokenChangeCounter.h"

#include "UserLogin.h"