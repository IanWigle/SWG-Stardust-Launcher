#pragma once
#include <vector>
#include <oleauto.h>

namespace FirebaseLib
{
	class ::firebase::App;

	class FirebaseLib_API FirebaseManager
	{
	public:
		FirebaseManager(const std::string& email, const std::string& password);
		explicit FirebaseManager();
		~FirebaseManager();

		virtual void Destroy();

		void Register();

		void Login();

		std::string& GetEmail();
		std::string& GetPassword();

		// Auth functions;
		void SetEmail(const char* userName);
		void SetPassword(const char* password);

		inline App* GetApp() { return m_App; }
		inline Auth* GetAuth() { return m_Auth; }

		bool StilSignedIn();
		
	private:
		// General App Stuff
		void SetupApp();
		App* m_App = nullptr;

		// Auth Stuff
		void SetupAuth();
		Auth* m_Auth;
		
		bool m_LogErrors;
		std::string m_Email;
		std::string m_Password;
		
		Future<User*> User;
	};

	EXPIMP_TEMPLATE template class FirebaseLib_API std::vector<FirebaseManager>;

	// START EXTERN DECLARATIONS
	typedef void(_stdcall* LPEXTFUNCRESPOND) (LPCSTR s);
	extern "C"
	{
		__declspec(dllexport) FirebaseManager* FirebaseManager_Create()
		{ 
			return new FirebaseManager(); 
		}
		__declspec(dllexport) FirebaseManager* FirebaseManager_CreateWithUserAndPassword(const char* username, const char* password)
		{ 
			return new FirebaseManager(username, password); 
		}
		__declspec(dllexport) void FirebaseManager_Delete(FirebaseManager* manager)
		{ 
			delete manager; 
		}
		__declspec(dllexport) void FirebaseManager_SetEmail(FirebaseManager* manager, const char* email) 
		{ 
			manager->SetEmail(email); 
		}
		__declspec(dllexport) void FirebaseManager_SetPassword(FirebaseManager* manager, const char* password) 
		{ 
			manager->SetPassword(password); 
		}
		__declspec(dllexport) void FirebaseManager_Register(FirebaseManager* manager) 
		{ 
			manager->Register(); 
		}
		__declspec(dllexport) void __stdcall FirebaseManager_GetPassword(FirebaseManager* manager, LPEXTFUNCRESPOND respond)
		{
			respond(manager->GetPassword().c_str());
		}
		__declspec(dllexport) void __stdcall FirebaseManager_GetEmail(FirebaseManager* manager, LPEXTFUNCRESPOND respond)
		{
			respond(manager->GetEmail().c_str());
		}
		__declspec(dllexport) bool FirebaseManager_StillSignedIn(FirebaseManager* manager)
		{
			return manager->StilSignedIn();
		}
		__declspec(dllexport) void FirebaseManager_SignOut(FirebaseManager* manager)
		{
			manager->GetAuth()->SignOut();
		}
	}
}

