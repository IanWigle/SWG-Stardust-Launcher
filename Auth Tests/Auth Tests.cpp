// Auth Tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "AuthTestsPCH.h"

int main(int argc, const char* argv[])
{
    std::cout << "Hello World!\n";

    ChangeToFileDirectory(
        FIREBASE_CONFIG_STRING[0] != '\0' ?
        FIREBASE_CONFIG_STRING : argv[0]);  // NOLINT
#ifdef _WIN32
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)SignalHandler, TRUE);
#else
    signal(SIGINT, SignalHandler);
#endif  // _WIN32

    App* app;
    LogMessage("Starting Auth tests.");

    app = App::Create();
    LogMessage("Created the Firebase app %x.",
        static_cast<int>(reinterpret_cast<intptr_t>(app)));

    // Create the Auth class for that App.

    ::firebase::ModuleInitializer initializer;
    initializer.Initialize(app, nullptr, [](::firebase::App* app, void*) {
        ::firebase::InitResult init_result;
        Auth::GetAuth(app, &init_result);
        return init_result;
        });

    while (initializer.InitializeLastResult().status() !=
        firebase::kFutureStatusComplete) {
        if (ProcessEvents(100)) return 1;  // exit if requested
    }

    if (initializer.InitializeLastResult().error() != 0) {
        LogMessage("Failed to initialize Auth: %s",
            initializer.InitializeLastResult().error_message());
        ProcessEvents(2000);
        return 1;
    }

    Auth* auth = Auth::GetAuth(app);

    LogMessage("Created the Auth %x class for the Firebase app.", static_cast<int>(reinterpret_cast<intptr_t>(auth)));

    // It's possible for current_user() to be non-null if the previous run
    // left us in a signed-in state.

    if (auth->current_user() == nullptr)
    {
        LogMessage("No user signed in at creation time.");
    }
    else
    {
        LogMessage("Current user uid(%s) name(%s) already signed in, so signing them out.",
                   auth->current_user()->uid().c_str(),
                   auth->current_user()->display_name().c_str());

        auth->SignOut();
    }

    // Credential copy tests
    {
        Credential email_cred =
            EmailAuthProvider::GetCredential(kCustomEmail, kCustomPassword);

        // Test copy constructor.
        Credential cred_copy(email_cred);
    }

    // Custom Profile Tests
    {
        if (kTestCustomEmail)
        {
            // Test the Auth::SignInWithEmailAndPassword().
            // Sign in with email and password that have already been registered.
            Future<User*> sign_in_future =
                auth->SignInWithEmailAndPassword(kCustomEmail, kCustomPassword);

            Wait::WaitForSignInFuture(sign_in_future,
                                      "Auth::SignInWithEmailAndPassword() existing "
                                      "(custom) email and password",
                                      kAuthErrorNone, auth);

            // Test the SignOut() after signed in with email and password.
            if (sign_in_future.status() == ::firebase::kFutureStatusComplete)
            {
                auth->SignOut();
                if (auth->current_user() != nullptr)
                {
                    LogMessage("ERROR: current_user() returning %x instead of nullptr after"
                               "SignOut()",
                               auth->current_user());
                }
            }
        }
    }

    // StateChange tests
    {
        AuthStateChangeCounter counter;
        IdTokenChangeCounter token_counter;

        // Test notification on registration.
        auth->AddAuthStateListener(&counter);
        auth->AddIdTokenListener(&token_counter);
        // Expect notification immediately after registration.
        counter.CompleteTest("registration", 1);
        token_counter.CompleteTest("registration", 1);

        // Test notification on SignOut(), when already signed-out.
        auth->SignOut();
        counter.CompleteTest("SignOut() when already signed-out", 0);
        token_counter.CompleteTest("SignOut() when already signed-out", 0);

        // Test notification on SignIn().
        Future<User*> sign_in_future = auth->SignInAnonymously();
        Wait::WaitForSignInFuture(sign_in_future, "Auth::SignInAnonymously()",
            kAuthErrorNone, auth);
        // Notified when the user is about to change and after the user has
        // changed.
        counter.CompleteTest("SignInAnonymously()", 1, 4);
        token_counter.CompleteTest("SignInAnonymously()", 1, 5);

        // Refresh the token.
        if (auth->current_user() != nullptr) {
            Future<std::string> token_future = auth->current_user()->GetToken(true);
            Wait::WaitForFuture(token_future, "GetToken()", kAuthErrorNone);
            counter.CompleteTest("GetToken()", 0);
            token_counter.CompleteTest("GetToken()", 1);
        }

        // Test notification on SignOut(), when signed-in.
        LogMessage("Current user %p", auth->current_user());  // DEBUG
        auth->SignOut();
        // Wait for the sign out to complete.
        Wait::WaitForSignOut(auth);
        counter.CompleteTest("SignOut()", 1);
        token_counter.CompleteTest("SignOut()", 1);
        LogMessage("Current user %p", auth->current_user());  // DEBUG

        auth->RemoveAuthStateListener(&counter);
        auth->RemoveIdTokenListener(&token_counter);
    }

    // --- Auth tests ------------------------------------------------------------
    {
        UserLogin user_login(auth);  // Generate a random name/password
        user_login.Register();
        if (!user_login.user()) {
            LogMessage("ERROR: Could not register new user.");
        }
        else {
            // Test Auth::SignInAnonymously().
            {
                Future<User*> sign_in_future = auth->SignInAnonymously();
                Wait::WaitForSignInFuture(sign_in_future, "Auth::SignInAnonymously()",
                    kAuthErrorNone, auth);
                ExpectTrue("SignInAnonymouslyLastResult matches returned Future",
                    sign_in_future == auth->SignInAnonymouslyLastResult());

                // Test SignOut() after signed in anonymously.
                if (sign_in_future.status() == ::firebase::kFutureStatusComplete) {
                    auth->SignOut();
                    if (auth->current_user() != nullptr) {
                        LogMessage(
                            "ERROR: current_user() returning %x instead of nullptr after "
                            "SignOut()",
                            auth->current_user());
                    }
                }
            }

            // Test Auth::FetchProvidersForEmail().
            {
                Future<Auth::FetchProvidersResult> providers_future =
                    auth->FetchProvidersForEmail(user_login.email());
                Wait::WaitForFuture(providers_future, "Auth::FetchProvidersForEmail()",
                    kAuthErrorNone);
                ExpectTrue(
                    "FetchProvidersForEmailLastResult matches returned Future",
                    providers_future == auth->FetchProvidersForEmailLastResult());

                const Auth::FetchProvidersResult* pro = providers_future.result();
                if (pro) {
                    LogMessage("  email %s, num providers %d", user_login.email(),
                        pro->providers.size());
                    for (auto it = pro->providers.begin(); it != pro->providers.end();
                        ++it) {
                        LogMessage("    * %s", it->c_str());
                    }
                }
            }

            // Test Auth::SignInWithEmailAndPassword().
            // Sign in with email and password that have already been registered.
            {
                Future<User*> sign_in_future = auth->SignInWithEmailAndPassword(
                    user_login.email(), user_login.password());
                Wait::WaitForSignInFuture(
                    sign_in_future,
                    "Auth::SignInWithEmailAndPassword() existing email and password",
                    kAuthErrorNone, auth);
                ExpectTrue(
                    "SignInWithEmailAndPasswordLastResult matches returned Future",
                    sign_in_future == auth->SignInWithEmailAndPasswordLastResult());

                // Test SignOut() after signed in with email and password.
                if (sign_in_future.status() == ::firebase::kFutureStatusComplete) {
                    auth->SignOut();
                    if (auth->current_user() != nullptr) {
                        LogMessage(
                            "ERROR: current_user() returning %x instead of nullptr after "
                            "SignOut()",
                            auth->current_user());
                    }
                }
            }

            // Test User::UpdateUserProfile
            {
                Future<User*> sign_in_future = auth->SignInWithEmailAndPassword(
                    user_login.email(), user_login.password());
                Wait::WaitForSignInFuture(
                    sign_in_future,
                    "Auth::SignInWithEmailAndPassword() existing email and password",
                    kAuthErrorNone, auth);
                if (sign_in_future.error() == kAuthErrorNone) {
                    User* user = *sign_in_future.result();
                    const char* kDisplayName = "Hello World";
                    const char* kPhotoUrl = "http://test.com/image.jpg";
                    User::UserProfile user_profile;
                    user_profile.display_name = kDisplayName;
                    user_profile.photo_url = kPhotoUrl;
                    Future<void> update_profile_future =
                        user->UpdateUserProfile(user_profile);
                    Wait::WaitForFuture(update_profile_future, "User::UpdateUserProfile",
                        kAuthErrorNone);
                    if (update_profile_future.error() == kAuthErrorNone) {
                        ExpectStringsEqual("User::display_name", kDisplayName,
                            user->display_name().c_str());
                        ExpectStringsEqual("User::photo_url", kPhotoUrl,
                            user->photo_url().c_str());
                    }
                }
            }

            // Sign in anonymously, link an email credential, reauthenticate with the
            // credential, unlink the credential and finally sign out.
            {
                Future<User*> sign_in_anonymously_future = auth->SignInAnonymously();
                Wait::WaitForSignInFuture(sign_in_anonymously_future,
                    "Auth::SignInAnonymously", kAuthErrorNone, auth);
                if (sign_in_anonymously_future.error() == kAuthErrorNone) {
                    User* user = *sign_in_anonymously_future.result();
                    std::string email = CreateNewEmail();
                    Credential credential =
                        EmailAuthProvider::GetCredential(email.c_str(), kTestPassword);
                    // Link with an email / password credential.
                    Future<SignInResult> link_future =
                        user->LinkAndRetrieveDataWithCredential(credential);
                    Wait::WaitForSignInFuture(link_future,
                        "User::LinkAndRetrieveDataWithCredential",
                        kAuthErrorNone, auth);
                    if (link_future.error() == kAuthErrorNone) {
                        Log::LogSignInResult(*link_future.result());
                        Future<SignInResult> reauth_future =
                            user->ReauthenticateAndRetrieveData(credential);
                        Wait::WaitForSignInFuture(reauth_future,
                            "User::ReauthenticateAndRetrieveData",
                            kAuthErrorNone, auth);
                        if (reauth_future.error() == kAuthErrorNone) {
                            Log::LogSignInResult(*reauth_future.result());
                        }
                        // Unlink email / password from credential.
                        Future<User*> unlink_future =
                            user->Unlink(credential.provider().c_str());
                        Wait::WaitForSignInFuture(unlink_future, "User::Unlink", kAuthErrorNone,
                            auth);
                    }
                    auth->SignOut();
                }
            }

            // Sign in user with bad email. Should fail.
            {
                Future<User*> sign_in_future_bad_email =
                    auth->SignInWithEmailAndPassword(kTestEmailBad, kTestPassword);
                Wait::WaitForSignInFuture(sign_in_future_bad_email,
                    "Auth::SignInWithEmailAndPassword() bad email",
                    ::firebase::auth::kAuthErrorUserNotFound, auth);
            }

            // Sign in user with correct email but bad password. Should fail.
            {
                Future<User*> sign_in_future_bad_password =
                    auth->SignInWithEmailAndPassword(user_login.email(),
                        kTestPasswordBad);
                Wait::WaitForSignInFuture(sign_in_future_bad_password,
                    "Auth::SignInWithEmailAndPassword() bad password",
                    ::firebase::auth::kAuthErrorWrongPassword, auth);
            }

            // Try to create with existing email. Should fail.
            {
                Future<User*> create_future_bad = auth->CreateUserWithEmailAndPassword(
                    user_login.email(), user_login.password());
                Wait::WaitForSignInFuture(
                    create_future_bad,
                    "Auth::CreateUserWithEmailAndPassword() existing email",
                    ::firebase::auth::kAuthErrorEmailAlreadyInUse, auth);
                ExpectTrue(
                    "CreateUserWithEmailAndPasswordLastResult matches returned Future",
                    create_future_bad ==
                    auth->CreateUserWithEmailAndPasswordLastResult());
            }

            // Test Auth::SignInWithCredential() using email&password.
            // Use existing email. Should succeed.
            {
                Credential email_cred_ok = EmailAuthProvider::GetCredential(
                    user_login.email(), user_login.password());
                Future<User*> sign_in_cred_ok =
                    auth->SignInWithCredential(email_cred_ok);
                Wait::WaitForSignInFuture(sign_in_cred_ok,
                    "Auth::SignInWithCredential() existing email",
                    kAuthErrorNone, auth);
                ExpectTrue("SignInWithCredentialLastResult matches returned Future",
                    sign_in_cred_ok == auth->SignInWithCredentialLastResult());
            }

            // Test Auth::SignInAndRetrieveDataWithCredential using email & password.
            // Use existing email. Should succeed.
            {
                Credential email_cred = EmailAuthProvider::GetCredential(
                    user_login.email(), user_login.password());
                Future<SignInResult> sign_in_future =
                    auth->SignInAndRetrieveDataWithCredential(email_cred);
                Wait::WaitForSignInFuture(sign_in_future,
                    "Auth::SignInAndRetrieveDataWithCredential "
                    "existing email",
                    kAuthErrorNone, auth);
                ExpectTrue(
                    "SignInAndRetrieveDataWithCredentialLastResult matches "
                    "returned Future",
                    sign_in_future ==
                    auth->SignInAndRetrieveDataWithCredentialLastResult());
                if (sign_in_future.error() == kAuthErrorNone) {
                    const SignInResult* sign_in_result = sign_in_future.result();
                    if (sign_in_result != nullptr && sign_in_result->user) {
                        LogMessage("SignInAndRetrieveDataWithCredential");
                        Log::LogSignInResult(*sign_in_result);
                    }
                    else {
                        LogMessage(
                            "ERROR: SignInAndRetrieveDataWithCredential returned no "
                            "result");
                    }
                }
            }

#if FALSE
            // Use bad Facebook credentials. Should fail.
            {
                Credential facebook_cred_bad =
                    FacebookAuthProvider::GetCredential(kTestAccessTokenBad);
                Future<User*> facebook_bad =
                    auth->SignInWithCredential(facebook_cred_bad);
                Wait::WaitForSignInFuture(
                    facebook_bad,
                    "Auth::SignInWithCredential() bad Facebook credentials",
                    kAuthErrorInvalidCredential, auth);
            }

            // Use bad GitHub credentials. Should fail.
            {
                Credential git_hub_cred_bad =
                    GitHubAuthProvider::GetCredential(kTestAccessTokenBad);
                Future<User*> git_hub_bad =
                    auth->SignInWithCredential(git_hub_cred_bad);
                Wait::WaitForSignInFuture(
                    git_hub_bad, "Auth::SignInWithCredential() bad GitHub credentials",
                    kAuthErrorInvalidCredential, auth);
            }

            // Use bad Google credentials. Should fail.
            {
                Credential google_cred_bad = GoogleAuthProvider::GetCredential(
                    kTestIdTokenBad, kTestAccessTokenBad);
                Future<User*> google_bad = auth->SignInWithCredential(google_cred_bad);
                Wait::WaitForSignInFuture(
                    google_bad, "Auth::SignInWithCredential() bad Google credentials",
                    kAuthErrorInvalidCredential, auth);
            }

            // Use bad Google credentials, missing an optional parameter. Should fail.
            {
                Credential google_cred_bad =
                    GoogleAuthProvider::GetCredential(kTestIdTokenBad, nullptr);
                Future<User*> google_bad = auth->SignInWithCredential(google_cred_bad);
                Wait::WaitForSignInFuture(
                    google_bad, "Auth::SignInWithCredential() bad Google credentials",
                    kAuthErrorInvalidCredential, auth);
            }

            // Use bad Twitter credentials. Should fail.
            {
                Credential twitter_cred_bad = TwitterAuthProvider::GetCredential(
                    kTestIdTokenBad, kTestAccessTokenBad);
                Future<User*> twitter_bad =
                    auth->SignInWithCredential(twitter_cred_bad);
                Wait::WaitForSignInFuture(
                    twitter_bad, "Auth::SignInWithCredential() bad Twitter credentials",
                    kAuthErrorInvalidCredential, auth);
            }

            // Construct OAuthCredential with nonce & access token.
            {
                Credential nonce_credential_good =
                    OAuthProvider::GetCredential(kTestIdProviderIdBad, kTestIdTokenBad,
                        kTestNonceBad, kTestAccessTokenBad);
            }

            // Construct OAuthCredential with nonce, null access token.
            {
                Credential nonce_credential_good = OAuthProvider::GetCredential(
                    kTestIdProviderIdBad, kTestIdTokenBad, kTestNonceBad,
                    /*access_token=*/nullptr);
            }

            // Use bad OAuth credentials. Should fail.
            {
                Credential oauth_cred_bad = OAuthProvider::GetCredential(
                    kTestIdProviderIdBad, kTestIdTokenBad, kTestAccessTokenBad);
                Future<User*> oauth_bad = auth->SignInWithCredential(oauth_cred_bad);
                Wait::WaitForSignInFuture(
                    oauth_bad, "Auth::SignInWithCredential() bad OAuth credentials",
                    kAuthErrorFailure, auth);
            }

            // Use bad OAuth credentials with nonce. Should fail.
            {
                Credential oauth_cred_bad =
                    OAuthProvider::GetCredential(kTestIdProviderIdBad, kTestIdTokenBad,
                        kTestNonceBad, kTestAccessTokenBad);
                Future<User*> oauth_bad = auth->SignInWithCredential(oauth_cred_bad);
                Wait::WaitForSignInFuture(
                    oauth_bad, "Auth::SignInWithCredential() bad OAuth credentials",
                    kAuthErrorFailure, auth);
            }
#endif

            // Test Auth::SendPasswordResetEmail().
            // Use existing email. Should succeed.
            {
                Future<void> send_password_reset_ok =
                    auth->SendPasswordResetEmail(user_login.email());
                Wait::WaitForFuture(send_password_reset_ok,
                    "Auth::SendPasswordResetEmail() existing email",
                    kAuthErrorNone);
                ExpectTrue(
                    "SendPasswordResetEmailLastResult matches returned Future",
                    send_password_reset_ok == auth->SendPasswordResetEmailLastResult());
            }

            // Use bad email. Should fail.
            {
                Future<void> send_password_reset_bad =
                    auth->SendPasswordResetEmail(kTestEmailBad);
                Wait::WaitForFuture(send_password_reset_bad,
                    "Auth::SendPasswordResetEmail() bad email",
                    ::firebase::auth::kAuthErrorUserNotFound);
            }
        }
    }
    // --- User tests ------------------------------------------------------------
    // Test anonymous user info strings.
    {
        Future<User*> anon_sign_in_for_user = auth->SignInAnonymously();
        Wait::WaitForSignInFuture(anon_sign_in_for_user,
            "Auth::SignInAnonymously() for User", kAuthErrorNone,
            auth);
        if (anon_sign_in_for_user.status() == ::firebase::kFutureStatusComplete) {
            User* anonymous_user = anon_sign_in_for_user.result()
                ? *anon_sign_in_for_user.result()
                : nullptr;
            if (anonymous_user != nullptr) {
                LogMessage("Anonymous uid is %s", anonymous_user->uid().c_str());
                ExpectStringsEqual("Anonymous user email", "",
                    anonymous_user->email().c_str());
                ExpectStringsEqual("Anonymous user display_name", "",
                    anonymous_user->display_name().c_str());
                ExpectStringsEqual("Anonymous user photo_url", "",
                    anonymous_user->photo_url().c_str());
                ExpectStringsEqual("Anonymous user provider_id", kFirebaseProviderId,
                    anonymous_user->provider_id().c_str());
                ExpectTrue("Anonymous user is_anonymous()",
                    anonymous_user->is_anonymous());
                ExpectFalse("Anonymous user is_email_verified()",
                    anonymous_user->is_email_verified());
                ExpectTrue("Anonymous user metadata().last_sign_in_timestamp != 0",
                    anonymous_user->metadata().last_sign_in_timestamp != 0);
                ExpectTrue("Anonymous user metadata().creation_timestamp != 0",
                    anonymous_user->metadata().creation_timestamp != 0);

                // Test User::LinkWithCredential(), linking with email & password.
                const std::string newer_email = CreateNewEmail();
                Credential user_cred = EmailAuthProvider::GetCredential(
                    newer_email.c_str(), kTestPassword);
                {
                    Future<User*> link_future =
                        anonymous_user->LinkWithCredential(user_cred);
                    Wait::WaitForSignInFuture(link_future, "User::LinkWithCredential()",
                        kAuthErrorNone, auth);
                }

                // Test User::LinkWithCredential(), linking with same email & password.
                {
                    Future<User*> link_future =
                        anonymous_user->LinkWithCredential(user_cred);
                    Wait::WaitForSignInFuture(link_future, "User::LinkWithCredential() again",
                        ::firebase::auth::kAuthErrorProviderAlreadyLinked,
                        auth);
                }

                // Test User::LinkWithCredential(), linking with bad credential.
                // Call should fail and Auth's current user should be maintained.
                {
                    const User* pre_link_user = auth->current_user();
                    ExpectTrue("Test precondition requires active user",
                        pre_link_user != nullptr);

                    Credential twitter_cred_bad = TwitterAuthProvider::GetCredential(
                        kTestIdTokenBad, kTestAccessTokenBad);
                    Future<User*> link_bad_future =
                        anonymous_user->LinkWithCredential(twitter_cred_bad);
                    Wait::WaitForFuture(link_bad_future,
                        "User::LinkWithCredential() with bad credential",
                        kAuthErrorInvalidCredential);
                    ExpectTrue("Linking maintains user",
                        auth->current_user() == pre_link_user);
                }

                // Test Auth::SignInWithCredential(), signing in with bad credential.
                // Call should fail, and Auth's current user should be maintained.
                {
                    const User* pre_signin_user = auth->current_user();
                    ExpectTrue("Test precondition requires active user",
                        pre_signin_user != nullptr);
                    Credential twitter_cred_bad = TwitterAuthProvider::GetCredential(
                        kTestIdTokenBad, kTestAccessTokenBad);
                    Future<User*> signin_bad_future =
                        auth->SignInWithCredential(twitter_cred_bad);
                    Wait::WaitForFuture(signin_bad_future,
                        "Auth::SignInWithCredential() with bad credential",
                        kAuthErrorInvalidCredential, auth);
                    ExpectTrue("Failed sign in maintains user",
                        auth->current_user() == pre_signin_user);
                }

                UserLogin user_login(auth);
                user_login.Register();

                if (!user_login.user()) {
                    LogMessage("Error - Could not create new user.");
                }
                else {
                    // Test email user info strings.
                    Future<User*> email_sign_in_for_user =
                        auth->SignInWithEmailAndPassword(user_login.email(),
                            user_login.password());
                    Wait::WaitForSignInFuture(email_sign_in_for_user,
                        "Auth::SignInWithEmailAndPassword() for User",
                        kAuthErrorNone, auth);
                    User* email_user = email_sign_in_for_user.result()
                        ? *email_sign_in_for_user.result()
                        : nullptr;
                    if (email_user != nullptr) {
                        LogMessage("Email uid is %s", email_user->uid().c_str());
                        ExpectStringsEqual("Email user email", user_login.email(),
                            email_user->email().c_str());
                        ExpectStringsEqual("Email user display_name", "",
                            email_user->display_name().c_str());
                        ExpectStringsEqual("Email user photo_url", "",
                            email_user->photo_url().c_str());
                        ExpectStringsEqual("Email user provider_id", kFirebaseProviderId,
                            email_user->provider_id().c_str());
                        ExpectFalse("Email user is_anonymous()",
                            email_user->is_anonymous());
                        ExpectFalse("Email user is_email_verified()",
                            email_user->is_email_verified());
                        ExpectTrue("Email user metadata().last_sign_in_timestamp != 0",
                            email_user->metadata().last_sign_in_timestamp != 0);
                        ExpectTrue("Email user metadata().creation_timestamp  != 0",
                            email_user->metadata().creation_timestamp != 0);

                        // Test User::GetToken().
                        // with force_refresh = false.
                        Future<std::string> token_no_refresh = email_user->GetToken(false);
                        Wait::WaitForFuture(token_no_refresh, "User::GetToken(false)",
                            kAuthErrorNone);
                        LogMessage("User::GetToken(false) = %s",
                            token_no_refresh.result()
                            ? token_no_refresh.result()->c_str()
                            : "");

                        // with force_refresh = true.
                        Future<std::string> token_force_refresh =
                            email_user->GetToken(true);
                        Wait::WaitForFuture(token_force_refresh, "User::GetToken(true)",
                            kAuthErrorNone);
                        LogMessage("User::GetToken(true) = %s",
                            token_force_refresh.result()
                            ? token_force_refresh.result()->c_str()
                            : "");

                        // Test Reload().
                        Future<void> reload_future = email_user->Reload();
                        Wait::WaitForFuture(reload_future, "User::Reload()", kAuthErrorNone);

                        // Test User::Unlink().
                        Future<User*> unlink_future = email_user->Unlink("firebase");
                        Wait::WaitForSignInFuture(unlink_future, "User::Unlink()",
                            ::firebase::auth::kAuthErrorNoSuchProvider,
                            auth);

                        // Sign in again if user is now invalid.
                        if (auth->current_user() == nullptr) {
                            Future<User*> email_sign_in_again =
                                auth->SignInWithEmailAndPassword(user_login.email(),
                                    user_login.password());
                            Wait::WaitForSignInFuture(email_sign_in_again,
                                "Auth::SignInWithEmailAndPassword() again",
                                kAuthErrorNone, auth);
                            email_user = email_sign_in_again.result()
                                ? *email_sign_in_again.result()
                                : nullptr;
                        }
                    }
                    if (email_user != nullptr) {
                        // Test User::provider_data().
                        const std::vector<UserInfoInterface*>& provider_data =
                            email_user->provider_data();
                        LogMessage("User::provider_data() returned %d interface%s",
                            provider_data.size(),
                            provider_data.size() == 1 ? "" : "s");
                        for (size_t i = 0; i < provider_data.size(); ++i) {
                            const UserInfoInterface* user_info = provider_data[i];
                            LogMessage(
                                "    UID() = %s\n"
                                "    Email() = %s\n"
                                "    DisplayName() = %s\n"
                                "    PhotoUrl() = %s\n"
                                "    ProviderId() = %s",
                                user_info->uid().c_str(), user_info->email().c_str(),
                                user_info->display_name().c_str(),
                                user_info->photo_url().c_str(),
                                user_info->provider_id().c_str());
                        }

                        // Test User::UpdateEmail().
                        const std::string newest_email = CreateNewEmail();
                        Future<void> update_email_future =
                            email_user->UpdateEmail(newest_email.c_str());
                        Wait::WaitForFuture(update_email_future, "User::UpdateEmail()",
                            kAuthErrorNone);

                        // Test User::UpdatePassword().
                        Future<void> update_password_future =
                            email_user->UpdatePassword(kTestPasswordUpdated);
                        Wait::WaitForFuture(update_password_future, "User::UpdatePassword()",
                            kAuthErrorNone);

                        // Test User::Reauthenticate().
                        Credential email_cred_reauth = EmailAuthProvider::GetCredential(
                            newest_email.c_str(), kTestPasswordUpdated);
                        Future<void> reauthenticate_future =
                            email_user->Reauthenticate(email_cred_reauth);
                        Wait::WaitForFuture(reauthenticate_future, "User::Reauthenticate()",
                            kAuthErrorNone);

                        // Test User::SendEmailVerification().
                        Future<void> send_email_verification_future =
                            email_user->SendEmailVerification();
                        Wait::WaitForFuture(send_email_verification_future,
                            "User::SendEmailVerification()", kAuthErrorNone);
                    }
                }
            }
        }

        // Test User::Delete().
        const std::string new_email_for_delete = CreateNewEmail();
        Future<User*> create_future_for_delete =
            auth->CreateUserWithEmailAndPassword(new_email_for_delete.c_str(),
                kTestPassword);
        Wait::WaitForSignInFuture(
            create_future_for_delete,
            "Auth::CreateUserWithEmailAndPassword() new email for delete",
            kAuthErrorNone, auth);
        User* email_user_for_delete = create_future_for_delete.result()
            ? *create_future_for_delete.result()
            : nullptr;
        if (email_user_for_delete != nullptr) {
            Future<void> delete_future = email_user_for_delete->Delete();
            Wait::WaitForFuture(delete_future, "User::Delete()", kAuthErrorNone);
        }
    }
    {
        // We end with a login so that we can test if a second run will detect
        // that we're already logged-in.
        Future<User*> sign_in_future = auth->SignInAnonymously();
        Wait::WaitForSignInFuture(sign_in_future, "Auth::SignInAnonymously() at end",
            kAuthErrorNone, auth);

        LogMessage("Anonymous uid(%s)", auth->current_user()->uid().c_str());
    }

    LogMessage("Completed Auth tests.");

    while (!ProcessEvents(1000)) {
    }

    delete auth;
    delete app;

    return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
