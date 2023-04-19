#pragma once

//disable warnings on 255 char debug symbols
#pragma warning (disable : 4786)

//disable warnings on extern before template instantiation
#pragma warning (disable : 4231)

// disable warning on define macro expecting '('
#pragma warning (disable : 4081)

// For now, disable warnings related to dll-interfacing for class members.
#pragma warning (disable : 4275)
#pragma warning (disable : 4251)

#ifdef FIREBASELIB_EXPORTS
#define FirebaseLib_API __declspec(dllexport)
#define EXPIMP_TEMPLATE
#else
#define FirebaseLib_API __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#endif // FIREBASELIB_EXPORTS

#ifdef FIREBASE_CONFIG
#define FIREBASE_CONFIG_STRING TO_STRING(FIREBASE_CONFIG)
#else
#define FIREBASE_CONFIG_STRING ""
#endif  // FIREBASE_CONFIG

// add headers that you want to pre-compile here
#include "framework.h"

#include "firebase/app.h"
#include "firebase/auth.h"
#include "firebase/auth/user.h"
#include "firebase/auth/credential.h"
#include "firebase/database.h"
#include "firebase/future.h"
#include "firebase/util.h"

using ::firebase::App;
using ::firebase::AppOptions;
using ::firebase::Future;
using ::firebase::FutureBase;
using ::firebase::Variant;
// Authentication namespaces
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
// Realtime Database namespaces
using ::firebase::database::Database;
using ::firebase::database::DatabaseReference;

static void ChangeToFileDirectory(const char* file_path) {
    std::string path(file_path);
    std::replace(path.begin(), path.end(), '\\', '/');
    auto slash = path.rfind('/');
    if (slash != std::string::npos) {
        std::string directory = path.substr(0, slash);
        if (!directory.empty()) chdir(directory.c_str());
    }
}

static void LogMessage(const char* format, ...) {
    va_list list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
    printf("\n");
    fflush(stdout);
}

// Wait for a Future to be completed. If the Future returns an error, it will
// be logged.
static void WaitForCompletion(const firebase::FutureBase& future, const char* name) {
    while (future.status() == firebase::kFutureStatusPending) {
        ProcessEvents(100);
    }
    if (future.status() != firebase::kFutureStatusComplete) {
        LogMessage("ERROR: %s returned an invalid result.", name);
    }
    else if (future.error() != 0) {
        LogMessage("ERROR: %s returned error %d: %s", name, future.error(),
            future.error_message());
    }
}