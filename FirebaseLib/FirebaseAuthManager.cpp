#include "pch.h"
#include "FirebaseAuthManager.h"

FirebaseLib::Auth::FireBaseAuthManager::FireBaseAuthManager(const std::string& email, const std::string& password) :
    m_Auth(nullptr),
    m_Email(email),
    m_Password(password)
{
#ifdef _DEBUG
    m_LogErrors = true;
#else
    m_LogErrors = false;
#endif // DEBUG
}

FirebaseLib::Auth::FireBaseAuthManager::FireBaseAuthManager()
{
    m_Email = "";
    m_Password = "";
    m_Auth = nullptr;
#ifdef _DEBUG
    m_LogErrors = true;
#else
    m_LogErrors = false;
#endif // DEBUG
}

void FirebaseLib::Auth::FireBaseAuthManager::Destroy()
{
}

void FirebaseLib::Auth::FireBaseAuthManager::Register()
{
}

void FirebaseLib::Auth::FireBaseAuthManager::Login()
{
}

const char* FirebaseLib::Auth::FireBaseAuthManager::GetEmail() const
{
    return nullptr;
}

const char* FirebaseLib::Auth::FireBaseAuthManager::GetPassword() const
{
    return nullptr;
}

void FirebaseLib::Auth::FireBaseAuthManager::SetEmail(const char* email)
{
}

void FirebaseLib::Auth::FireBaseAuthManager::SetPassword(const char* password)
{
}

const char* FirebaseLib::Auth::FireBaseAuthManager::GetManagerName() const
{
    return "FireBaseAuthManager";
}

void FirebaseLib::Auth::FireBaseAuthManager::SetupAuth()
{
}
