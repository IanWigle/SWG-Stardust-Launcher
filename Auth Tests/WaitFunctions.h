#pragma once

class Wait {
public:
	static bool WaitForFuture(const FutureBase& future, const char* fn, AuthError expected_error, bool log_error = true);

	static bool WaitForSignInFuture(Future<User*> sign_in_future, const char* fn, AuthError expected_error, Auth* auth);

	static bool WaitForSignInFuture(const Future<SignInResult>& sign_in_future, const char* fn, AuthError expected_error, Auth* auth);

	// Wait for the current user to sign out.  Typically you should use the
	// state listener to determine whether the user has signed out.
	static bool WaitForSignOut(firebase::auth::Auth* auth);
};
