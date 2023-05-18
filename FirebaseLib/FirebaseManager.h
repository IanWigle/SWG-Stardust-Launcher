#pragma once

#define HANDLE_CURRENT_ACCOUNT(c) switch(c) { \
							      case Type::Anonymous : DeleteCurrentAccount(); break; \
							      default: SignOut(); break; }

#define SAFE_DELETE(p) { delete p; p = nullptr; }

namespace FirebaseLib
{
	static const char* kStorageUrl = nullptr;

	namespace UserType
	{
		enum Type
		{
			Error,
			Anonymous,
			EmailVerified
		};
	}

	class FirebaseLib_API FirebaseManager
	{
	public:
		FirebaseManager();
		~FirebaseManager();

		// Auth functions;
		void SetEmail(const char* userName);
		void SetPassword(const char* password);
		void SetDisplayName(const char* name);


		bool Login();
		bool Register();
		void SignOut();

		void SendPasswordReset(const char* emailAddress);

		std::string& GetEmail();
		std::string& GetPassword();
		std::string& GetDisplayName();

		bool StilSignedIn();

		void SignInAnon();
		UserType::Type GetAccountType();

		void SetupPhoneAuthentication(const char* phoneNumber);

		inline AuthError GetLastAuthError() { return LastAuthError; }
		inline std::string GetLastErrorString() { return LastErrorString; }

		std::string GetUserId();

		// Database functions;
		std::string GetLauncherVersion();
		std::string GetGameVersion();

		// Cloud Storage functions;
		void DownloadLauncher();
		void DownloadGameUpdate();
		void DownloadGame();

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
		Database* m_Database;
		std::string m_SavedUrl;		

		// Cloud Storage
		void SetupCloudStorage();
		Storage* m_Storage;
		StorageReference m_CloudRootReference;
		std::string RootString = "Root";
		std::string LauncherString = "SWGLauncher.zip";
	};

	EXPIMP_TEMPLATE FirebaseLib_API std::vector<FirebaseManager>;

	// START EXTERN DECLARATIONS
	typedef void(_stdcall* LPEXTFUNCRESPOND) (LPCSTR s);
	extern "C"
	{
		FirebaseLib_API FirebaseManager* FirebaseManager_Create()
		{ 
			return new FirebaseManager(); 
		}
		FirebaseLib_API void FirebaseManager_Delete(FirebaseManager* manager)
		{ 
			delete manager; 
		}
		FirebaseLib_API void FirebaseManager_SetEmail(FirebaseManager* manager, const char* email)
		{ 
			manager->SetEmail(email); 
		}
		FirebaseLib_API void FirebaseManager_SetPassword(FirebaseManager* manager, const char* password)
		{ 
			manager->SetPassword(password); 
		}
		FirebaseLib_API bool FirebaseManager_Register(FirebaseManager* manager)
		{ 
			return manager->Register(); 
		}
		FirebaseLib_API void __stdcall FirebaseManager_GetPassword(FirebaseManager* manager, LPEXTFUNCRESPOND respond)
		{
			respond(manager->GetPassword().c_str());
		}
		FirebaseLib_API void __stdcall FirebaseManager_GetEmail(FirebaseManager* manager, LPEXTFUNCRESPOND respond)
		{
			respond(manager->GetEmail().c_str());
		}
		FirebaseLib_API bool FirebaseManager_StillSignedIn(FirebaseManager* manager)
		{
			return manager->StilSignedIn();
		}
		FirebaseLib_API void FirebaseManager_SignOut(FirebaseManager* manager)
		{
			manager->SignOut();
		}
		FirebaseLib_API void FirebaseManager_SignInAnon(FirebaseManager* manager)
		{
			manager->SignInAnon();
		}
		FirebaseLib_API void __stdcall FirebaseManager_GetClientVersion(FirebaseManager* manager, LPEXTFUNCRESPOND respond)
		{
			respond(manager->GetLauncherVersion().c_str());
		}
		FirebaseLib_API int FirebaseManager_GetAccountType(FirebaseManager* manager)
		{
			return manager->GetAccountType();
		}
		FirebaseLib_API void FirebaseManager_DownloadSWGLauncher(FirebaseManager* manager)
		{
			manager->DownloadLauncher();
		}
		FirebaseLib_API int FirebaseManager_GetLastAuthError(FirebaseManager* manager)
		{
			return manager->GetLastAuthError();
		}
		FirebaseLib_API void __stdcall FirebaseManager_GetLastError(FirebaseManager* manager, LPEXTFUNCRESPOND respond)
		{
			respond(manager->GetLastErrorString().c_str());
		}
		FirebaseLib_API bool FirebaseManager_Login(FirebaseManager* manager)
		{
			return manager->Login();
		}
		// refer to types.h in firebase::auth::AuthError
		FirebaseLib_API int FirebaseManager_GetAuthError(FirebaseManager* manager)
		{
			return (int)manager->GetLastAuthError();
		}
		FirebaseLib_API void __stdcall FirebaseManager_GetUserId(FirebaseManager* manager, LPEXTFUNCRESPOND respond)
		{
			respond(manager->GetUserId().c_str());
		}
		FirebaseLib_API void FirebaseManager_SendPasswordResetEmail(FirebaseManager* manager, const char* email)
		{
			manager->SendPasswordReset(email);
		}
		FirebaseLib_API void FirebaseManager_SetDisplayName(FirebaseManager* manager, const char* name)
		{
			manager->SetDisplayName(name);
		}
		FirebaseLib_API void __stdcall FirebaseManager_GetDisplayName(FirebaseManager* manager, LPEXTFUNCRESPOND respond)
		{
			respond(manager->GetDisplayName().c_str());
		}
		FirebaseLib_API void __stdcall FirebaseManager_GetGameVersion(FirebaseManager* manager, LPEXTFUNCRESPOND respond)
		{
			respond(manager->GetGameVersion().c_str());
		}
		FirebaseLib_API void FirebaseManager_DownloadGameUpdate(FirebaseManager* manager)
		{
			manager->DownloadGameUpdate();
		}
		FirebaseLib_API void FirebaseManager_DownloadGame(FirebaseManager* manager)
		{
			manager->DownloadGame();
		}
	}
}

