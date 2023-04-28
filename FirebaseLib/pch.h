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
#define EXPIMP_TEMPLATE
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

static void LogVariantMap(const std::map<Variant, Variant>& variant_map, int indent);
static void LogVariantVector(const std::vector<Variant>& variants, int indent);
static void LogSignInResult(const SignInResult& result);

static void LogVariantVector(const std::vector<Variant>& variants, int indent) {
    std::string indent_string(indent * 2, ' ');
    LogMessage("%s[", indent_string.c_str());
    for (auto it = variants.begin(); it != variants.end(); ++it) {
        const Variant& item = *it;
        if (item.is_fundamental_type()) {
            const Variant& string_value = item.AsString();
            LogMessage("%s  %s,", indent_string.c_str(), string_value.string_value());
        }
        else if (item.is_vector()) {
            LogVariantVector(item.vector(), indent + 2);
        }
        else if (item.is_map()) {
            LogVariantMap(item.map(), indent + 2);
        }
        else {
            LogMessage("%s  ERROR: unknown type %d", indent_string.c_str(),
                static_cast<int>(item.type()));
        }
    }
    LogMessage("%s]", indent_string.c_str());
}

// Log a map of variants.
static void LogVariantMap(const std::map<Variant, Variant>& variant_map,
    int indent) {
    std::string indent_string(indent * 2, ' ');
    for (auto it = variant_map.begin(); it != variant_map.end(); ++it) {
        const Variant& key_string = it->first.AsString();
        const Variant& value = it->second;
        if (value.is_fundamental_type()) {
            const Variant& string_value = value.AsString();
            LogMessage("%s%s: %s,", indent_string.c_str(), key_string.string_value(),
                string_value.string_value());
        }
        else {
            LogMessage("%s%s:", indent_string.c_str(), key_string.string_value());
            if (value.is_vector()) {
                LogVariantVector(value.vector(), indent + 1);
            }
            else if (value.is_map()) {
                LogVariantMap(value.map(), indent + 1);
            }
            else {
                LogMessage("%s  ERROR: unknown type %d", indent_string.c_str(),
                    static_cast<int>(value.type()));
            }
        }
    }
}

// Display the sign-in result.
static void LogSignInResult(const SignInResult& result) {
    if (!result.user) {
        LogMessage("ERROR: User not signed in");
        return;
    }
    LogMessage("* User ID %s", result.user->uid().c_str());
    const AdditionalUserInfo& info = result.info;
    LogMessage("* Provider ID %s", info.provider_id.c_str());
    LogMessage("* User Name %s", info.user_name.c_str());
    LogVariantMap(info.profile, 0);
    const UserMetadata& metadata = result.meta;
    LogMessage("* Sign in timestamp %d",
        static_cast<int>(metadata.last_sign_in_timestamp));
    LogMessage("* Creation timestamp %d",
        static_cast<int>(metadata.creation_timestamp));
}
#pragma endregion Logging

#pragma region Wait Calls
// Don't return until `future` is complete.
// Print a message for whether the result mathes our expectations.
// Returns true if the application should exit.
static bool WaitForFuture(const FutureBase& future, const char* fn, AuthError expected_error, bool log_error = true);
static bool WaitForSignInFuture(Future<User*> sign_in_future, const char* fn, AuthError expected_error, Auth* auth);
static bool WaitForSignInFuture(const Future<SignInResult>& sign_in_future, const char* fn, AuthError expected_error, Auth* auth);
// Wait for the current user to sign out.  Typically you should use the
// state listener to determine whether the user has signed out.
static bool WaitForSignOut(firebase::auth::Auth* auth);

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

bool WaitForSignInFuture(Future<User*> sign_in_future, const char* fn,
    AuthError expected_error, Auth* auth) {
    if (WaitForFuture(sign_in_future, fn, expected_error)) return true;

    const User* const* sign_in_user_ptr = sign_in_future.result();
    const User* sign_in_user =
        sign_in_user_ptr == nullptr ? nullptr : *sign_in_user_ptr;
    const User* auth_user = auth->current_user();

    if (expected_error == ::firebase::auth::kAuthErrorNone &&
        sign_in_user != auth_user) {
        LogMessage("ERROR: future's user (%x) and current_user (%x) don't match",
            static_cast<int>(reinterpret_cast<intptr_t>(sign_in_user)),
            static_cast<int>(reinterpret_cast<intptr_t>(auth_user)));
    }

    return false;
}

bool WaitForSignInFuture(const Future<SignInResult>& sign_in_future,
    const char* fn, AuthError expected_error,
    Auth* auth) {
    if (WaitForFuture(sign_in_future, fn, expected_error)) return true;

    const SignInResult* sign_in_result = sign_in_future.result();
    const User* sign_in_user = sign_in_result ? sign_in_result->user : nullptr;
    const User* auth_user = auth->current_user();

    if (expected_error == ::firebase::auth::kAuthErrorNone &&
        sign_in_user != auth_user) {
        LogMessage("ERROR: future's user (%x) and current_user (%x) don't match",
            static_cast<int>(reinterpret_cast<intptr_t>(sign_in_user)),
            static_cast<int>(reinterpret_cast<intptr_t>(auth_user)));
    }

    return false;
}

bool WaitForSignOut(firebase::auth::Auth* auth) {
    while (auth->current_user() != nullptr) {
        if (ProcessEvents(100)) return true;
    }
    // Wait - hopefully - long enough for listeners to be signalled.
    ProcessEvents(1000);
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

#pragma region Expect Functions
static void ExpectFalse(const char* test, bool value) {
    if (value) {
        LogMessage("ERROR: %s is true instead of false", test);
    }
    else {
        LogMessage("%s is false, as expected", test);
    }
}

static void ExpectTrue(const char* test, bool value) {
    if (value) {
        LogMessage("%s is true, as expected", test);
    }
    else {
        LogMessage("ERROR: %s is false instead of true", test);
    }
}

// Log results of a string comparison for `test`.
static void ExpectStringsEqual(const char* test, const char* expected,
    const char* actual) {
    if (strcmp(expected, actual) == 0) {
        LogMessage("%s is '%s' as expected", test, actual);
    }
    else {
        LogMessage("ERROR: %s is '%s' instead of '%s'", test, actual, expected);
    }
}
#pragma endregion Expect Functions

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
#pragma endregion StaticIntegers

#pragma region StaticFlags
extern "C" static bool logToDisk = false;
#pragma endregion StaticFlags
#endif //PCH_H
