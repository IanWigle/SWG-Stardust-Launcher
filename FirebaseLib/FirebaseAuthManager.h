#pragma once
#include <vector>

namespace FirebaseLib::Auth
{
	class Auth;

	//EXPIMP_TEMPLATE template class FirebaseLib_API std::_String_val<std::_Simple_types<char>>;
	//EXPIMP_TEMPLATE template class FirebaseLib_API std::_Compressed_pair<std::allocator<char>, std::_String_val<std::_Simple_types<char>>, true>;
	//EXPIMP_TEMPLATE template class FirebaseLib_API std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

	class FirebaseLib_API FireBaseAuthManager : public Manager
	{
	public:
		FireBaseAuthManager(const std::string& email, const std::string& password);

		explicit FireBaseAuthManager();

		virtual void Destroy() override;

		void Register();

		void Login();

		const char* GetEmail() const;
		const char* GetPassword() const;

		void SetEmail(const char* email);
		void SetPassword(const char* password);

		virtual const char* GetManagerName() const;

	private:
		void SetupAuth();

		Auth* m_Auth;
		std::string m_Email;
		std::string m_Password;
		bool m_LogErrors;
	};

	//EXPIMP_TEMPLATE template class FirebaseLib_API std::vector<int>;
	//EXPIMP_TEMPLATE template class FirebaseLib_API std::vector<char>;
	//EXPIMP_TEMPLATE template class FirebaseLib_API std::vector<std::string>;
	//EXPIMP_TEMPLATE template class FirebaseLib_API std::allocator<FireBaseAuthManager>;
	//EXPIMP_TEMPLATE template class FirebaseLib_API std::vector<FireBaseAuthManager>;
}

