#include "pch.h"

FirebaseLib::FirebaseManager::FirebaseManager(const std::string& email, const std::string& password) :
    m_Auth(nullptr),
    m_App(nullptr),
    m_Email(email),
    m_Password(password)
{
#ifdef _DEBUG
    m_LogErrors = true;
#else
    m_LogErrors = false;
#endif // DEBUG
}

FirebaseLib::FirebaseManager::FirebaseManager()
{
    m_Email = "";
    m_Password = "";
    m_Auth = nullptr;
    m_App = nullptr;
    SetupApp();
    SetupAuth();
}

FirebaseLib::FirebaseManager::~FirebaseManager()
{
    Destroy();
}

void FirebaseLib::FirebaseManager::Destroy()
{
    delete m_Auth;
    m_Auth = nullptr;

    delete m_App;
    m_App = nullptr;
}

void FirebaseLib::FirebaseManager::Register()
{
    User = m_Auth->CreateUserWithEmailAndPassword(m_Email.c_str(), m_Password.c_str());
    
    //WaitForSignInFuture(register_account,
    //                    "CreateUserWithEmailAndPassword() to create user",
    //                    kAuthErrorNone,
    //                    m_Auth);
    //Future<::firebase::auth::User*> sign_in_anonymously_future = m_Auth->SignInAnonymously();
    
}

void FirebaseLib::FirebaseManager::Login()
{
}

std::string& FirebaseLib::FirebaseManager::GetEmail()
{
    return m_Email;
}

std::string& FirebaseLib::FirebaseManager::GetPassword()
{
    return m_Password;
}

void FirebaseLib::FirebaseManager::SetEmail(const char* userName)
{
    m_Email = userName;
}

void FirebaseLib::FirebaseManager::SetPassword(const char* password)
{
    m_Password = password;
}

bool FirebaseLib::FirebaseManager::StilSignedIn()
{
    return m_Auth->current_user() != nullptr;
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

    ::firebase::ModuleInitializer initializer;
    initializer.Initialize(m_App, nullptr, [](::firebase::App* app, void*) {
        ::firebase::InitResult init_result;
        Auth::GetAuth(app, &init_result);
        return init_result;
        });

    while (initializer.InitializeLastResult().status() !=
        firebase::kFutureStatusComplete) {
        if (ProcessEvents(100)) return;  // exit if requested
    }

    if (initializer.InitializeLastResult().error() != 0) {
        LogMessage("Failed to initialize Auth: %s",
            initializer.InitializeLastResult().error_message());
        ProcessEvents(2000);
        return;
    }
}

void FirebaseLib::FirebaseManager::SetupAuth()
{
    m_Auth = Auth::GetAuth(m_App);
}
