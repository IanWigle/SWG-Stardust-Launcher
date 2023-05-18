// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

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
#define EXPIMP_TEMPLATE template class
#else
#define FirebaseLib_API __declspec(dllimport)
#define EXPIMP_TEMPLATE extern
#endif // FIREBASELIB_EXPORTS

// add headers that you want to pre-compile here
#include "framework.h"

#include "firebase/app.h"
#include "firebase/auth.h"
#include "firebase/auth/user.h"
#include "firebase/auth/credential.h"
#include "firebase/database.h"
#include "firebase/future.h"
#include "firebase/util.h"
#include "firebase/storage.h"

// General App namespaces
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
using ::firebase::auth::kAuthErrorFailure;
using ::firebase::auth::kAuthErrorInvalidCredential;
using ::firebase::auth::kAuthErrorInvalidProviderId;
using ::firebase::auth::kAuthErrorNone;
using ::firebase::auth::OAuthProvider;
using ::firebase::auth::SignInResult;
using ::firebase::auth::User;
using ::firebase::auth::UserInfoInterface;
using ::firebase::auth::UserMetadata;
using ::firebase::auth::PhoneAuthProvider;
// Realtime Database namespaces
using ::firebase::database::Database;
using ::firebase::database::DatabaseReference;
// Cloud Storage namespaces
using ::firebase::storage::Storage;
using ::firebase::storage::StorageReference;
using ::firebase::storage::Error;
using ::firebase::storage::Error::kErrorUnknown;
using ::firebase::storage::Error::kErrorObjectNotFound;

#ifdef FIREBASE_CONFIG
#define FIREBASE_CONFIG_STRING TO_STRING(FIREBASE_CONFIG)
#else
#define FIREBASE_CONFIG_STRING ""
#endif  // FIREBASE_CONFIG

#pragma region Logging
static void LogMessage(const char* format, ...)
{
    va_list list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
    printf("\n");
    fflush(stdout);
}
#pragma endregion Logging

#pragma region Wait Calls
// Don't return until `future` is complete.
// Print a message for whether the result mathes our expectations.
// Returns true if the application should exit.
static bool WaitForFuture(const FutureBase& future, const char* fn, AuthError expected_error, bool log_error = true);

bool WaitForFuture(const FutureBase& future, const char* fn,
    AuthError expected_error, bool log_error) {
    // Note if the future has not be started properly.
    if (future.status() == ::firebase::kFutureStatusInvalid) {
        LogMessage("ERROR: Future for %s is invalid", fn);
        return false;
    }

    // Wait for future to complete.
    LogMessage("  Calling %s...", fn);
    while (future.status() == ::firebase::kFutureStatusPending) {
        if (ProcessEvents(100)) return true;
    }

    // Log error result.
    if (log_error) {
        const AuthError error = static_cast<AuthError>(future.error());
        if (error == expected_error) {
            const char* error_message = future.error_message();
            if (error_message) {
                LogMessage("%s completed as expected", fn);
            }
            else {
                LogMessage("%s completed as expected, error: %d '%s'", fn, error,
                    error_message);
            }
        }
        else {
            LogMessage("ERROR: %s completed with error: %d, `%s`", fn, error,
                future.error_message());
        }
    }
    return false;
}

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
};

#pragma endregion Wait Calls

#pragma region Custom Classes
#include "FirebaseManager.h"
#pragma endregion Custom Classes

#pragma region StaticStrings
extern "C" static const char ManagerSTR[] = "FireBaseManager";
extern "C" static const char AuthManagerSTR[] = "FireBaseAuthManager";
#pragma endregion StaticStrings

#pragma region StaticIntegers
extern "C" static const int PhoneWaitIntervalMs = 300;
// Max time to wait for phone authentication to complete
extern "C" static const int PhoneAuthCodeSendWaitMs = 600000;
extern "C" static const int NumGameFiles = 10;
#pragma endregion StaticIntegers

#pragma region StaticDoubles
// Max time is i x 600. 
extern "C" static const double MaxDownloadSpeed = 18000;
#pragma endregion StaticDoubles

#pragma region StaticFlags
extern "C" static bool logToDisk = false;
#pragma endregion StaticFlags
#endif //PCH_H
