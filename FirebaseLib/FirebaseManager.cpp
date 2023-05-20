#include "pch.h"
#include "PhoneListener.h"
#include "DownloadListener.h"

using FirebaseLib::UserType::Type;

const char* FirebaseLib::FirebaseManager::kStorageUrl = nullptr;
FirebaseLib::FirebaseManager::FirebaseManager() :
    m_Email(""),
    m_Password(""),
    m_Auth(nullptr),
    m_App(nullptr)
{
    m_Logger = new Logger();
    SetupApp();
    SetupAuth();
    SetupDatabase();
    SetupCloudStorage();
}

FirebaseLib::FirebaseManager::~FirebaseManager()
{
    Destroy();
}

int FirebaseLib::FirebaseManager::NumberOfGameZips()
{
    LogFirebase("FirebaseManager::NumberOfGameZips() : Getting Number of zips for full game.");

    firebase::database::DatabaseReference ref = m_Database->GetReference("ClientData");

    firebase::Future<firebase::database::DataSnapshot> f1 =
        ref.Child("NumberOfGameZips").GetValue();

    WaitForCompletion(f1, "FirebaseManager::NumberOfGameZips()");

    LogFirebase("FirebaseManager::NumberOfGameZips() : There are %i zips for the full game to download.", f1.result()->value().int64_value());
    return f1.result()->value().int64_value();
}

void FirebaseLib::FirebaseManager::LogLauncher(const char* log)
{
    m_Logger->LogLauncher(log);
}

void FirebaseLib::FirebaseManager::Destroy()
{
    HANDLE_CURRENT_ACCOUNT

    SAFE_DELETE(m_Logger);

    SAFE_DELETE(m_Storage);

    SAFE_DELETE(m_Database);

    SAFE_DELETE(m_Auth);

    SAFE_DELETE(m_App);
}

void FirebaseLib::FirebaseManager::LogFirebase(const char* log, ...)
{
    m_Logger->LogFirebase(log);
}

bool FirebaseLib::FirebaseManager::Register()
{
    HANDLE_CURRENT_ACCOUNT

    Future<::firebase::auth::User*> sign_in_future =
        m_Auth->CreateUserWithEmailAndPassword(m_Email.c_str(), m_Password.c_str());
    WaitForFuture(sign_in_future, "FirebaseManager::Register()", kAuthErrorNone);
    m_Auth->current_user()->SendEmailVerification();

    const AuthError error = static_cast<AuthError>(sign_in_future.error());
    Future<void> update_future;
    switch (error)
    {
    case kAuthErrorNone:
    {
        m_LastAuthError = kAuthErrorNone;
        m_SignedUser = sign_in_future;

        User::UserProfile profile;
        profile.display_name = m_DisplayName.c_str();

        update_future = m_Auth->current_user()->UpdateUserProfile(profile);

        WaitForFuture(update_future, "FirebaseManager::Register()::UpdateProfile", kAuthErrorNone);
        if(static_cast<AuthError>(update_future.error()) != kAuthErrorNone)
        {
            m_LastAuthErrorString = update_future.error_message();
            m_LastAuthError = error;
            return false;
            goto Failure;
        }

        return true;
    }        
    default:
        m_LastAuthErrorString = sign_in_future.error_message();
        m_LastAuthError = error;
        LogFirebase("FirebaseManager::Register() : %s", sign_in_future.error_message());
        goto Failure;
    }

Failure:
    LogFirebase("FirebaseManager::Register() : Finished call.");
        return false;
    }
}

bool FirebaseLib::FirebaseManager::Login()
{
    if (StilSignedIn())
        HANDLE_CURRENT_ACCOUNT

    //m_Auth->
    Future<User*> sign_in_future =
        m_Auth->SignInWithEmailAndPassword(m_Email.c_str(), m_Password.c_str());

    WaitForFuture(sign_in_future, "FirebaseManager::Login()", kAuthErrorNone);
    
    const AuthError error = static_cast<AuthError>(sign_in_future.error());
    switch (error)
    {
    case kAuthErrorNone:
        m_LastAuthError = kAuthErrorNone;
        m_SignedUser = sign_in_future;
        break;
    default:
        m_LastAuthErrorString = sign_in_future.error_message();
        m_LastAuthError = error;
        SignOut();
        return false;
    }

    if (!m_Auth->current_user()->is_email_verified())
    {
        m_LastAuthErrorString = "FirebaseManager::Login() : Not verified account! Please check your email.";
        m_LastAuthError = firebase::auth::kAuthErrorUnverifiedEmail;
        SignOut();
        return false;
    }

    m_DisplayName = m_Auth->current_user()->display_name();

    return true;
}

void FirebaseLib::FirebaseManager::SignOut()
{
    if (StilSignedIn())
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

bool FirebaseLib::FirebaseManager::StilSignedIn()
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
        Type::Error;
    else
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

    return f1.result()->value().string_value();
}

std::string FirebaseLib::FirebaseManager::GetGameVersion()
{
    firebase::database::DatabaseReference ref = m_Database->GetReference("ClientData");

    firebase::Future<firebase::database::DataSnapshot> f1 =
        ref.Child("GameVersion").GetValue();

    WaitForCompletion(f1, "FirebaseManager::GetGameVersion()");

    return f1.result()->value().string_value();
}

void FirebaseLib::FirebaseManager::DownloadLauncher()
{    
    //StorageReference fileRef = m_CloudRootReference.Child("SWGLauncher.zip");
    DownloadListener listener;
    Future<size_t> fileFuture = m_CloudRootReference.GetFile("SWGLauncher.zip", &listener, nullptr);
    WaitForCompletion(fileFuture, "FirebaseManager::DownloadLauncher()");

    const AuthError error = static_cast<AuthError>(fileFuture.error());
    m_LastAuthError = error;
    m_LastAuthErrorString = fileFuture.error_message();
}

void FirebaseLib::FirebaseManager::DownloadGameUpdate()
{
    TCHAR NPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, NPath);

    std::wstring wpath = NPath;
    std::string spath(wpath.begin(), wpath.end());

    LogMessage(spath.c_str());

    std::string filename = "StardustUpdate" + std::to_string(version) + ".zip";

    std::string fullpath = spath + "\\" + filename;

    StorageReference fileRef = m_CloudRootReference.Child(filename);

    DownloadListener listener;

    Future<size_t> fileFuture = fileRef.GetFile(fullpath.c_str(), &listener, nullptr);
    WaitForCompletion(fileFuture, "FirebaseManager::DownloadGameUpdate()");

    const StorageError error = static_cast<::firebase::storage::Error>(fileFuture.error());
    m_LastAuthErrorString = fileFuture.error_message();

    if (static_cast<::firebase::storage::Error>(fileFuture.error()) != ::firebase::storage::Error::kErrorNone)
    {
        m_LastAuthErrorString = fileFuture.error_message();
        m_LastStorageError = error;
        LogMessage("ERROR: %s", m_LastAuthErrorString.c_str());
    }
}

void FirebaseLib::FirebaseManager::DownloadGame()
{
    TCHAR NPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, NPath);

    std::wstring wpath = NPath;
    std::string spath(wpath.begin(), wpath.end());

    LogMessage(spath.c_str());

    int numgameFiles = NumberOfGameZips();
    for (int i = 1; i < numgameFiles + 1; i++)
    {
        std::string fileName = "StardustGameFiles" + std::to_string(i) + ".zip";
        std::string fullpath = spath + "\\" + fileName;

        StorageReference fileRef = m_CloudRootReference.Child(fileName);

        DownloadListener listener;

        Future<size_t> fileFuture = fileRef.GetFile(fullpath.c_str(), &listener, nullptr);
        LogMessage("Downloading %s", fileName.c_str());
        WaitForCompletion(fileFuture, "FirebaseManager::DownloadGame()");

        const ::firebase::storage::Error error = static_cast<::firebase::storage::Error>(fileFuture.error());
        m_LastAuthErrorString = fileFuture.error_message();
        if (error != ::firebase::storage::Error::kErrorNone)
        {
            LogMessage("ERROR: %s", m_LastAuthErrorString.c_str());
            LogFirebase("FirebaseManager::DownloadGameUpdate() : %s", m_LastAuthErrorString.c_str());
        }
    }
    LogFirebase("FirebaseManager::DownloadGame() : Finished Download");
    }

bool FirebaseLib::FirebaseManager::IsThereUpdate()
{
    StorageReference storageFile = m_CloudRootReference.Child("StardustUpdate.zip");

    return false;
}

void FirebaseLib::FirebaseManager::SetupApp()
{
    ChangeToFileDirectory(
        FIREBASE_CONFIG_STRING);  // NOLINT
#ifdef _WIN32
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)SignalHandler, TRUE);
#else
    signal(SIGINT, SignalHandler);
#endif  // _WIN32

    m_App = App::Create();
    LogMessage("Created the Firebase app %x.",
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
                firebase::storage::Storage::GetInstance(app, nullptr, &result);
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
