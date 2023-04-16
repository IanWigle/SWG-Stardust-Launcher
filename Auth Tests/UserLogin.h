#pragma once
#include "firebase/auth.h"

using ::firebase::auth::Auth;

class UserLogin
{
public:
	UserLogin(Auth* auth, const std::string& email, const std::string& password);

	explicit UserLogin(Auth* auth);

	~UserLogin();

	void Register();

	void Login();

	void Delete();

	const char* email() const;
	const char* password() const;
	User* user() const;
	void set_email(const char* email);
	void set_password(const char* password);

private:
	Auth* auth_;
	std::string email_;
	std::string password_;
	User* user_;
	bool log_errors_;
};