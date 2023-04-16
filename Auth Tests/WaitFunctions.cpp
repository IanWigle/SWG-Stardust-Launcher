#include "AuthTestsPCH.h"

// Don't return until `future` is complete.
// Print a message for whether the result mathes our expectations.
// Returns true if the application should exit.
bool Wait::WaitForFuture(const FutureBase& future, const char* fn,
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

bool Wait::WaitForSignInFuture(Future<User*> sign_in_future, const char* fn,
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

bool Wait::WaitForSignInFuture(const Future<SignInResult>& sign_in_future,
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

bool Wait::WaitForSignOut(firebase::auth::Auth* auth) {
    while (auth->current_user() != nullptr) {
        if (ProcessEvents(100)) return true;
    }
    // Wait - hopefully - long enough for listeners to be signalled.
    ProcessEvents(1000);
    return false;
}



