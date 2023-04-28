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

		// Auth functions;
		void SetEmail(const char* userName);
		void SetPassword(const char* password);
		void SetDisplayName(const char* name);

		bool Register();

		bool Login();
		void SignOut();

		void SendPasswordReset(const char* emailAddress);

		std::string& GetEmail();
		std::string& GetPassword();
		std::string& GetDisplayName();

		bool StilSignedIn();

		void SignInAnon();
		int GetAccountType();

		void SetupPhoneAuthentication(const char* phoneNumber);

		inline AuthError GetLastAuthError() { return LastAuthError; }
		inline std::string GetLastErrorString() { return LastErrorString; }

		std::string GetUserId();

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

		// Auth Stuff
		void SetupAuth();		
		void DeleteCurrentAccount();
		Auth* m_Auth;
		std::string m_Email;
		std::string m_Password;
		std::string m_DisplayName;
		AuthError LastAuthError;
		std::string LastErrorString;

		// Database Stuff
		void SetupDatabase();
		void AddAccountToDatabase();
		bool IsAccountAlreadyLoggedIn();
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
		__declspec(dllexport) bool FirebaseManager_Login(FirebaseManager* manager)
		{
			return manager->Login();
		}
		// refer to types.h in firebase::auth::AuthError
		__declspec(dllexport) int FirebaseManager_GetAuthError(FirebaseManager* manager)
		{
			return (int)manager->GetLastAuthError();
		}
		__declspec(dllexport) void __stdcall FirebaseManager_GetUserId(FirebaseManager* manager, LPEXTFUNCRESPOND respond)
		{
			respond(manager->GetUserId().c_str());
		}
		__declspec(dllexport) void FirebaseManager_SendPasswordResetEmail(FirebaseManager* manager, const char* email)
		{
			manager->SendPasswordReset(email);
		}
	}
}

