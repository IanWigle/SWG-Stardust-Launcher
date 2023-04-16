#include "AuthTestsPCH.h"

UserLogin::UserLogin(Auth* auth, const std::string& email, const std::string& password) :
    auth_(auth),
    email_(email),
    password_(password),
    user_(nullptr),
    log_errors_(true) {}

UserLogin::UserLogin(Auth* auth) :
    auth_(auth)
{
    email_ = CreateNewEmail();
    password_ = kTestPassword;
}

UserLogin::~UserLogin()
{
    if (user_ != nullptr)
    {
        log_errors_ = false;
        Delete();
    }
}

void UserLogin::Register()
{
    Future<User*> register_test_account = auth_->CreateUserWithEmailAndPassword(email(), password());

    Wait::WaitForSignInFuture(register_test_account,
                              "CreateUserWithEmailAndPassword() to create temp user",
                              kAuthErrorNone,
                              auth_);

    user_ = register_test_account.result() ? *register_test_account.result() : nullptr;
}

void UserLogin::Login()
{
    Credential email_cred = EmailAuthProvider::GetCredential(email(), password());
    Future<User*> sign_in_cred = auth_->SignInWithCredential(email_cred);
    Wait::WaitForSignInFuture(sign_in_cred,
                              "Auth::SignInWithCredential() for UserLogin",
                              kAuthErrorNone,
                              auth_);
}

void UserLogin::Delete()
{
    if (user_ != nullptr)
    {
        Future<void> delete_future = user_->Delete();
        if (delete_future.status() == ::firebase::kFutureStatusInvalid)
        {
            Login();
            delete_future = user_->Delete();
        }
    }
    user_ = nullptr;
}

const char* UserLogin::email() const
{
    return email_.c_str();
}

const char* UserLogin::password() const
{
    return password_.c_str();
}

User* UserLogin::user() const
{
    return user_;
}

void UserLogin::set_email(const char* email)
{
    email_ = email;
}

void UserLogin::set_password(const char* password)
{
    password_ = password;
}
