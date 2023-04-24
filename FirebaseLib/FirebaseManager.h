#pragma once
#include <vector>
#include <oleauto.h>

namespace FirebaseLib
{
	static const char* kStorageUrl = nullptr;

	class FirebaseLib_API FirebaseManager
	{
	public:
		FirebaseManager(const std::string& email, const std::string& password);
		explicit FirebaseManager();
		~FirebaseManager();

		bool Register();

		void Login();
		void SignOut();

		std::string& GetEmail();
		std::string& GetPassword();

		// Auth functions;
		void SetEmail(const char* userName);
		void SetPassword(const char* password);

		bool StilSignedIn();

		void DeleteCurrentAccount();

		void SignInAnon();
		int GetAccountType();

		inline AuthError GetLastAuthError() { return LastAuthError; }
		inline std::string GetLastErrorString() { return LastErrorString; }

		// Database functions;
		std::string GetLauncherVersion();

		// Cloud Storage functions;
		void DownloadLauncher();

	private:
		virtual void Destroy();

		// General App Stuff
		void SetupApp();
		App* m_App = nullptr;
		Future<User*> SignedUser;
		inline App* GetApp() { return m_App; }

		// Auth Stuff
		void SetupAuth();
		Auth* m_Auth;
		std::string m_Email;
		std::string m_Password;
		inline Auth* GetAuth() { return m_Auth; }
		AuthError LastAuthError;
		std::string LastErrorString;
		Future<User*> AnonUser;

		// Database Stuff
		void SetupDatabase();
		Database* m_Database;
		std::string m_SavedUrl;		

		// Cloud Storage
		void SetupCloudStorage();
		Storage* m_Storage;
		StorageReference m_CloudRootReference;
		std::string RootString = "Root";
		std::string LauncherString = "SWGLauncher.zip";
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
		__declspec(dllexport) bool FirebaseManager_Register(FirebaseManager* manager) 
		{ 
			return manager->Register(); 
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
			manager->SignOut();
		}
		__declspec(dllexport) void FirebaseManager_SignInAnon(FirebaseManager* manager)
		{
			manager->SignInAnon();
		}
		__declspec(dllexport) void __stdcall FirebaseManager_GetClientVersion(FirebaseManager* manager, LPEXTFUNCRESPOND respond)
		{
			respond(manager->GetLauncherVersion().c_str());
		}
		__declspec(dllexport) void FirebaseManager_DeleteCurrentAccount(FirebaseManager* manager)
		{
			manager->DeleteCurrentAccount();
		}
		__declspec(dllexport) int FirebaseManager_GetAccountType(FirebaseManager* manager)
		{
			return manager->GetAccountType();
		}
		__declspec(dllexport) void FirebaseManager_DownloadSWGLauncher(FirebaseManager* manager)
		{
			manager->DownloadLauncher();
		}
		__declspec(dllexport) int FirebaseManager_GetLastAuthError(FirebaseManager* manager)
		{
			return manager->GetLastAuthError();
		}
		__declspec(dllexport) void __stdcall FirebaseManager_GetLastError(FirebaseManager* manager, LPEXTFUNCRESPOND respond)
		{
			respond(manager->GetLastErrorString().c_str());
		}
	}
}

