#include "pch.h"
#include "PhoneListener.h"
#include "DownloadListener.h"

using FirebaseLib::UserType::Type;

const char* FirebaseLib::FirebaseManager::kStorageUrl = nullptr;
FirebaseLib::Logger* FirebaseLib::FirebaseManager::m_Logger = nullptr;

FirebaseLib::FirebaseManager::FirebaseManager() :
    m_Email(""),
    m_Password(""),
    m_Auth(nullptr),
    m_App(nullptr)
{
    m_Logger = new Logger();

    LogFirebase("FirebaseManager::Constructor() started.");
    SetupApp();
    SetupAuth();
    SetupDatabase();
    SetupCloudStorage();
    LogFirebase("FirebaseManager::Constructor() finished.");
}

FirebaseLib::FirebaseManager::~FirebaseManager()
{
    LogFirebase("FirebaseManager::Deconstructor() started.");
    Destroy();
    LogFirebase("FirebaseManager::Deconstructor() finished.");
    SAFE_DELETE(m_Logger);
}

int FirebaseLib::FirebaseManager::NumberOfGameZips()
{
    LogFirebase("FirebaseManager::NumberOfGameZips() : Getting Number of zips for full game.");

    firebase::database::DatabaseReference ref = m_Database->GetReference("ClientData");

    firebase::Future<firebase::database::DataSnapshot> f1 =
        ref.Child("NumberOfGameZips").GetValue();

    WaitForCompletion(f1, "FirebaseManager::NumberOfGameZips()");

    std::string message = "FirebaseManager::NumberOfGameZips() : There are " + std::to_string(f1.result()->value().int64_value()) + " zips for the full game to download.";
    LogFirebase(message);
    return f1.result()->value().int64_value();
}

bool FirebaseLib::FirebaseManager::UnderMaintenance()
{
    LogFirebase("FirebaseManager::UnderMaintenance() : Getting UnderMaintenance flag.");

    firebase::database::DatabaseReference ref = m_Database->GetReference("ClientData");

    firebase::Future<firebase::database::DataSnapshot> f1 =
        ref.Child("UnderMaintenance").GetValue();

    WaitForCompletion(f1, "FirebaseManager::UnderMaintenance()");

    return f1.result()->value().bool_value();
}

void FirebaseLib::FirebaseManager::LogLauncher(const char* log)
{
    m_Logger->LogLauncher(log);
}

FirebaseLib::Logger* FirebaseLib::FirebaseManager::GetLogger()
{
    return m_Logger;
}

void FirebaseLib::FirebaseManager::Destroy()
{
    HANDLE_CURRENT_ACCOUNT

    SAFE_DELETE(m_Storage);

    SAFE_DELETE(m_Database);

    SAFE_DELETE(m_Auth);

    SAFE_DELETE(m_App);
}

void FirebaseLib::FirebaseManager::LogFirebase(std::string message)
{
    m_Logger->LogFirebase(message.c_str());
}

bool FirebaseLib::FirebaseManager::Register()
{
    HANDLE_CURRENT_ACCOUNT

    Future<void> update_future;

    LogFirebase("FirebaseManager::Register() started.");

    Future<::firebase::auth::User*> sign_in_future =
        m_Auth->CreateUserWithEmailAndPassword(m_Email.c_str(), m_Password.c_str());
    WaitForFuture(sign_in_future, "FirebaseManager::Register()", kAuthErrorNone);

    LogFirebase("FirebaseManager::Register() : WaitForFuture to create account complete.");

    firebase::Future<void> emailVerFuture = m_Auth->current_user()->SendEmailVerification();
    LogFirebase("FirebaseManager::Register() : Sending Email Verification.");
    WaitForFuture(emailVerFuture,"FirebaseManager::Register()::SendEmailVerification", kAuthErrorNone);
    if (static_cast<AuthError>(emailVerFuture.error()) != kAuthErrorNone)
    {
        m_LastAuthError = static_cast<AuthError>(emailVerFuture.error());
        LogFirebase(emailVerFuture.error_message());
    }

    const AuthError error = static_cast<AuthError>(sign_in_future.error() | emailVerFuture.error());
    
    switch (error)
    {
    case kAuthErrorNone:
    {
        m_LastAuthError = kAuthErrorNone;

        User::UserProfile profile;
        profile.display_name = m_DisplayName.c_str();

        update_future = m_Auth->current_user()->UpdateUserProfile(profile);

        WaitForFuture(update_future, "FirebaseManager::Register()::UpdateProfile", kAuthErrorNone);

        LogFirebase("FirebaseManager::Register() : WaitForFuture to update account profile complete.");

        if(static_cast<AuthError>(update_future.error()) != kAuthErrorNone)
        {
            m_LastAuthErrorString = update_future.error_message();
            m_LastAuthError = error;

            LogFirebase("FirebaseManager::Register() : " + std::string(update_future.error_message()));

            goto Failure;
        }

        LogFirebase("FirebaseManager::Register() : Finished call.");
        return true;
    }        
    default:
        m_LastAuthErrorString = sign_in_future.error() == 0 ? emailVerFuture.error_message() :  sign_in_future.error_message();

        m_LastAuthError = error;
        LogFirebase("FirebaseManager::Register() : " + std::string(sign_in_future.error_message()));
        goto Failure;
    }

Failure:
    LogFirebase("FirebaseManager::Register() : Finished call.");
    return false;
}

bool FirebaseLib::FirebaseManager::Login()
{
    if (StillSignedIn())
        HANDLE_CURRENT_ACCOUNT

    LogFirebase("FirebaseManager::Login() started.");
    Future<User*> sign_in_future =
        m_Auth->SignInWithEmailAndPassword(m_Email.c_str(), m_Password.c_str());

    WaitForFuture(sign_in_future, "FirebaseManager::Login()", kAuthErrorNone);
    
    const AuthError error = static_cast<AuthError>(sign_in_future.error());
    switch (error)
    {
    case kAuthErrorNone:
        m_LastAuthError = kAuthErrorNone;
        break;
    default:
        m_LastAuthErrorString = sign_in_future.error_message();
        m_LastAuthError = error;
        SignOut();
        LogFirebase("FirebaseManager::Login() : " + std::string(sign_in_future.error_message()));
        return false;
    }

    if (!m_Auth->current_user()->is_email_verified())
    {
        m_LastAuthErrorString = "FirebaseManager::Login() : Not verified account! Please check your email.";
        m_LastAuthError = firebase::auth::kAuthErrorUnverifiedEmail;
        SignOut();
        //LogFirebase("FirebaseManager::Login() : Not verified account! Please check your email.");
        return false;
    }

    m_DisplayName = m_Auth->current_user()->display_name();

    //LogFirebase("FirebaseManager::Login() finished.");
    return true;
}

void FirebaseLib::FirebaseManager::SignOut()
{
    if (StillSignedIn())
    {
        switch (GetAccountType())
        {
            // Anonymous
        case 1:
            DeleteCurrentAccount();
            break;
        }
    }

    m_Auth->SignOut();
}

std::string& FirebaseLib::FirebaseManager::GetEmail()
{
    return m_Email;
}

std::string& FirebaseLib::FirebaseManager::GetPassword()
{
    return m_Password;
}

std::string& FirebaseLib::FirebaseManager::GetDisplayName()
{
    return m_DisplayName;
}

void FirebaseLib::FirebaseManager::SetEmail(const char* userName)
{
    m_Email = userName;
}

void FirebaseLib::FirebaseManager::SetPassword(const char* password)
{
    m_Password = password;
}

void FirebaseLib::FirebaseManager::SetDisplayName(const char* name)
{
    m_DisplayName = name;
}

bool FirebaseLib::FirebaseManager::StillSignedIn()
{
    return m_Auth->current_user() != nullptr;
}

void FirebaseLib::FirebaseManager::DeleteCurrentAccount()
{
    firebase::auth::User* user_to_delete = m_Auth->current_user();

    Future<void> delete_future = user_to_delete->Delete();
    WaitForFuture(delete_future, "FirebaseManager::DeleteCurrentAccount()", kAuthErrorNone);
}

void FirebaseLib::FirebaseManager::SignInAnon()
{
    HANDLE_CURRENT_ACCOUNT
    m_Auth->SignInAnonymously();
}

FirebaseLib::UserType::Type FirebaseLib::FirebaseManager::GetAccountType()
{
    if (m_Auth->current_user() == nullptr)
    {
        return Type::Error;
    }
    else if(m_Auth->current_user()->is_anonymous())
    {
        return Type::Anonymous;
    }
    else if (m_Auth->current_user()->is_email_verified())
    {
        return Type::EmailVerified;
    }

    return Type::Error;
}

void FirebaseLib::FirebaseManager::SetupPhoneAuthentication(const char* phoneNumber)
{
    PhoneListener listener;
    PhoneAuthProvider& phone_provider = PhoneAuthProvider::GetInstance(m_Auth);
    phone_provider.VerifyPhoneNumber(phoneNumber, 0,
        nullptr, &listener);

    int wait_ms = 0;
    while (listener.NumCallsOnVerificationComplete() == 0 &&
        listener.NumCallsOnVerificationFailed() == 0 &&
        listener.NumCallsOnCodeSent() == 0)
    {
        if (wait_ms > PhoneAuthCodeSendWaitMs)
        {
            break;
        }
        ProcessEvents(PhoneWaitIntervalMs);
        wait_ms += PhoneWaitIntervalMs;
        LogMessage(".");
    }
    if (wait_ms > PhoneAuthCodeSendWaitMs ||
        listener.NumCallsOnVerificationFailed())
    {
        LogMessage("FirebaseManager::SetupPhoneAuthentication() : ERROR: SMS with verification code not sent.");
        m_LastAuthError = kAuthErrorFailure;
        m_LastAuthErrorString = "FirebaseManager::SetupPhoneAuthentication() : SMS with verification code not sent.";
    }
    else
    {
        LogMessage("FirebaseManager::SetupPhoneAuthentication() : SMS verification code sent.");
    }
    // TODO:
    // We must now wait and get the code from the user's phone. Because we are implementing the
    // graphical forms on .net c#, we have to cut this part short. Once the user provides the code
    // needed to continue, the Form/Manager will go onto the second part in SetupPhoneAuthenticationPartTwo()
    // 
    // However, we are still time sensitive to ensure the code is still valid and not stale. The problem here
    // is that we would need to process events somewhere. The first idea I have is to create a second thread
    // that Processes Events similar as to what was done above. The thread would process and kill itself
    // should the waited millaseconds reached max wait time, or if the retrieval was successful according to 
    // the phone listener. This would also mean the listener is not a local variable within the function and is
    // instead created/stored as a class instance within the manager.
    //
    // A second other option is to continue in this function with another process events while-loop. However that 
    // would then require us to do some shenanigans in telling the c# to open the new form to then retrieve the
    // code. This option is not likely viable due to the dll to exe talk and that the application proccess would
    // be hung up on the process events while loop.
    // 
    // A third long, and more complicated possibility is making forms and windows in c++. This will take longer
    // because we'll have to setup the form in code. This also means we will also have to setup proccess and WndProcess
    // event listening for the window. It is possible for a DLL to be attached and listen to Wnd Events, I have done
    // game engines like that in the past for input and graphical OS events. If we do it this way we also prevent
    // the code from needing a second function to act as a part two. 
}

std::string FirebaseLib::FirebaseManager::GetUserId()
{
    return m_Auth->current_user()->uid();
}

std::string FirebaseLib::FirebaseManager::GetLauncherVersion()
{
    firebase::database::DatabaseReference ref = m_Database->GetReference("ClientData");

    firebase::Future<firebase::database::DataSnapshot> f1 =
        ref.Child("ClientVersion").GetValue();

    WaitForCompletion(f1, "FirebaseManager::GetLauncherVersion()");

    LogFirebase("FirebaseManager::GetLauncherVersion() : Version is " + std::string(f1.result()->value().string_value()));

    return f1.result()->value().string_value();
}

std::string FirebaseLib::FirebaseManager::GetGameVersion()
{
    firebase::database::DatabaseReference ref = m_Database->GetReference("ClientData");

    firebase::Future<firebase::database::DataSnapshot> f1 =
        ref.Child("GameVersion").GetValue();

    WaitForCompletion(f1, "FirebaseManager::GetGameVersion()");

    LogFirebase("FirebaseManager::GetGameVersion() : Version is " + std::string(f1.result()->value().string_value()));
    return f1.result()->value().string_value();
}

std::string FirebaseLib::FirebaseManager::GetUpdaterVersion()
{
    firebase::database::DatabaseReference ref = m_Database->GetReference("ClientData");

    firebase::Future<firebase::database::DataSnapshot> f1 =
        ref.Child("UpdaterVersion").GetValue();

    WaitForCompletion(f1, "FirebaseManager::GetUpdaterVersion()");

    LogFirebase("FirebaseManager::GetUpdaterVersion() : Version is " + std::string(f1.result()->value().string_value()));
    return f1.result()->value().string_value();
}

void FirebaseLib::FirebaseManager::DownloadLauncher()
{
    std::string fullpath = GetCurrentDirectoryPath() + "\\" + m_LauncherString;

    LogFirebase("FirebaseManager::DownloadLauncher() : Starting Download");
    DownloadListener listener;

    StorageReference fileRef = m_CloudRootReference.Child(m_LauncherString);
    Future<size_t> fileFuture = fileRef.GetFile(fullpath.c_str(), &listener, nullptr);
    WaitForCompletion(fileFuture, "FirebaseManager::DownloadLauncher()");

    const StorageError error = static_cast<::firebase::storage::Error>(fileFuture.error());
    m_LastStorageError = error;
    m_LastStorageErrorString = fileFuture.error_message();
    LogFirebase("FirebaseManager::DownloadLauncher() : Finished Download");
}

void FirebaseLib::FirebaseManager::DownloadGameUpdate(int version)
{
    LogFirebase("FirebaseManager::DownloadGameUpdate() : Starting Download");

    std::string filename = m_GameUpdateString + std::to_string(version) + ".zip";

    std::string fullpath = GetCurrentDirectoryPath() + "\\" + filename;

    StorageReference fileRef = m_CloudRootReference.Child(filename);

    DownloadListener listener;

    Future<size_t> fileFuture = fileRef.GetFile(fullpath.c_str(), &listener, nullptr);
    LogFirebase("FirebaseManager::DownloadGameUptade() downloading " + filename);
    WaitForCompletion(fileFuture, "FirebaseManager::DownloadGameUpdate()");
    
    m_LastStorageErrorString = fileFuture.error_message();
    m_LastStorageError = static_cast<::firebase::storage::Error>(fileFuture.error());

    if (m_LastStorageError != ::firebase::storage::Error::kErrorNone)
    {
        LogMessage("ERROR: %s", m_LastStorageErrorString.c_str());
        LogFirebase("FirebaseManager::DownloadGameUpdate() : " + std::string(fileFuture.error_message()));
    }
    LogFirebase("FirebaseManager::DownloadGameUpdate() : Finished Download");
}

void FirebaseLib::FirebaseManager::DownloadGame()
{
    LogFirebase("FirebaseManager::DownloadGame() : Starting Download");

    std::string path = GetCurrentDirectoryPath();

    int numgameFiles = NumberOfGameZips();
    for (int i = 1; i < numgameFiles + 1; i++)
    {
        std::string fileName = m_GameFilePartString + std::to_string(i) + ".zip";
        std::string fullpath = path + "\\" + fileName;

        StorageReference fileRef = m_CloudRootReference.Child(fileName);

        DownloadListener listener;

        Future<size_t> fileFuture = fileRef.GetFile(fullpath.c_str(), &listener, nullptr);
        LogFirebase("FirebaseManager::DownloadGame() : Downloading " + fileName);
        WaitForCompletion(fileFuture, "FirebaseManager::DownloadGame()");

        m_LastStorageError = static_cast<::firebase::storage::Error>(fileFuture.error());;
        m_LastStorageErrorString = fileFuture.error_message();

        if (m_LastStorageError != ::firebase::storage::Error::kErrorNone)
        {
            LogMessage("ERROR: %s", m_LastStorageErrorString.c_str());
            LogFirebase("FirebaseManager::DownloadGameUpdate() : " + m_LastStorageErrorString);
        }
    }
    LogFirebase("FirebaseManager::DownloadGame() : Finished Download");
}

void FirebaseLib::FirebaseManager::DownloadNewUpdater()
{
    std::string path = GetCurrentDirectoryPath();

    std::string fullpath = GetCurrentDirectoryPath() + "\\" + m_UpdaterString;

    StorageReference fileRef = m_CloudRootReference.Child(m_UpdaterString);

    DownloadListener listener;

    Future<size_t> fileFuture = fileRef.GetFile(fullpath.c_str(), &listener, nullptr);
    LogFirebase("FirebaseManager::DownloadNewUpdater() downloading " + m_UpdaterString);
    WaitForCompletion(fileFuture, "FirebaseManager::DownloadNewUpdater()");

    m_LastStorageError = static_cast<::firebase::storage::Error>(fileFuture.error());
    m_LastStorageErrorString = fileFuture.error_message();

    if (m_LastStorageError != ::firebase::storage::Error::kErrorNone)
    {
        LogMessage("ERROR: %s", m_LastStorageErrorString.c_str());
        LogFirebase("FirebaseManager::DownloadGameUpdate() : " + std::string(fileFuture.error_message()));
    }
    LogFirebase("FirebaseManager::DownloadGameUpdate() : Finished Download");
}

void FirebaseLib::FirebaseManager::SetupApp()
{
    LogFirebase("FirebaseManager::SetupApp() : Starting Firebase setup");
    ChangeToFileDirectory(
        FIREBASE_CONFIG_STRING);  // NOLINT
#ifdef _WIN32
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)SignalHandler, TRUE);
#else
    signal(SIGINT, SignalHandler);
#endif  // _WIN32

    m_App = App::Create();
    LogMessage("FirebaseManager::SetupApp() : Created the Firebase app %x.",
        static_cast<int>(reinterpret_cast<intptr_t>(m_App)));

    void* initialize_targets[] = { &m_Auth, &m_Database, &m_Storage };

    const firebase::ModuleInitializer::InitializerFn initializers[] = {
      [](::firebase::App* app, void* data) {
        LogMessage("FirebaseManager::SetupApp() : Attempt to initialize Firebase Auth.");
        void** targets = reinterpret_cast<void**>(data);
        ::firebase::InitResult result;
        *reinterpret_cast<::firebase::auth::Auth**>(targets[0]) =
            ::firebase::auth::Auth::GetAuth(app, &result);
        return result;
      },
      [](::firebase::App* app, void* data) {
        LogMessage("FirebaseManager::SetupApp() : Attempt to initialize Firebase Database.");
        void** targets = reinterpret_cast<void**>(data);
        ::firebase::InitResult result;
        *reinterpret_cast<::firebase::database::Database**>(targets[1]) =
            ::firebase::database::Database::GetInstance(app, &result);
        return result;
      },
      [](::firebase::App* app, void* data) {
            LogMessage("FirebaseManager::SetupApp() : Attempt to initialize Cloud Storage.");
            void** targets = reinterpret_cast<void**>(data);
            ::firebase::InitResult result;
            firebase::storage::Storage* storage =
                firebase::storage::Storage::GetInstance(app, kStorageUrl, &result);
            *reinterpret_cast<::firebase::storage::Storage**>(targets[2]) = storage;
            LogMessage("FirebaseManager::SetupApp() : Initialized storage with URL %s, %s",
                kStorageUrl ? kStorageUrl : "(null)",
                storage->url().c_str());
            return result;
        } };

    ::firebase::ModuleInitializer initializer;
    initializer.Initialize(m_App, initialize_targets, initializers,
        sizeof(initializers) / sizeof(initializers[0]));

    WaitForCompletion(initializer.InitializeLastResult(), "FirebaseManager::SetupApp()");
    LogFirebase("FirebaseManager::SetupApp() : Finished Firebase setup");
}

void FirebaseLib::FirebaseManager::SetupAuth()
{
    m_Auth = Auth::GetAuth(m_App);
}

void FirebaseLib::FirebaseManager::SendPasswordReset(const char* emailAddress)
{
    Future<void> future = m_Auth->SendPasswordResetEmail(emailAddress);
    WaitForCompletion(future,"FirebaseManager::SendPasswordReset()");

    const AuthError error = static_cast<AuthError>(future.error());
    switch (error)
    {
    case kAuthErrorNone:
        m_LastAuthError = kAuthErrorNone;
        break;
    default:
        m_LastAuthErrorString = future.error_message();
        m_LastAuthError = error;
    }
}

void FirebaseLib::FirebaseManager::SetupDatabase()
{
    m_SavedUrl = m_Database->url();
    m_Database->set_persistence_enabled(false);
}

void FirebaseLib::FirebaseManager::SetupCloudStorage()
{
    m_CloudRootReference = m_Storage->GetReference("Root/");
    m_Storage->set_max_download_retry_time(MaxDownloadSpeed);
    m_Storage->set_max_operation_retry_time(MaxDownloadSpeed);
}

std::string FirebaseLib::FirebaseManager::GetCurrentDirectoryPath()
{
    TCHAR NPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, NPath);

    std::wstring wpath = NPath;
    std::string spath(wpath.begin(), wpath.end());

    return spath;
}
