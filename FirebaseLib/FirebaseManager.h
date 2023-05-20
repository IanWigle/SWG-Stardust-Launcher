#pragma once

#define HANDLE_CURRENT_ACCOUNT    switch(GetAccountType()) { \
							      case UserType::Anonymous : DeleteCurrentAccount(); break; \
							      default: SignOut(); break; }

#define SAFE_DELETE(p) { delete p; p = nullptr; }

namespace FirebaseLib
{
	typedef ::firebase::storage::Error StorageError;
	typedef ::firebase::database::Error DatabaseError;

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

		inline AuthError GetLastAuthError() { return m_LastAuthError; }
		inline std::string GetLastAuthErrorString() { return m_LastAuthErrorString; }

		std::string GetUserId();

		// Database functions;
		std::string GetLauncherVersion();
		std::string GetGameVersion();
		int NumberOfGameZips();

		inline DatabaseError GetLastDatabaseError() { return m_LastDatabaseError; }
		inline std::string GetLastDatabaseErrorString() { return m_LastDatabaseErrorString; }

		// Cloud Storage functions;
		void DownloadLauncher();
		void DownloadGameUpdate(int version);
		void DownloadGame();
		bool IsThereUpdate();

		inline StorageError GetLastStorageError() { return m_LastStorageError; }
		inline std::string GetLastStorageErrorString() { return m_LastAuthErrorString; }

		void LogLauncher(const char* log);

	private:
		virtual void Destroy();

		// General App Stuff
		void SetupApp();
		App* m_App = nullptr;
		Future<User*> m_SignedUser;
		Logger* m_Logger = nullptr;

		// Auth Stuff
		void SetupAuth();		
		void DeleteCurrentAccount();
		Auth* m_Auth;
		std::string m_Email;
		std::string m_Password;
		std::string m_DisplayName;
		AuthError m_LastAuthError;
		std::string m_LastAuthErrorString;

		// Database Stuff
		void SetupDatabase();
		Database* m_Database;
		std::string m_SavedUrl;		
		std::string m_LastDatabaseErrorString;
		DatabaseError m_LastDatabaseError;

		// Cloud Storage
		void SetupCloudStorage();
		Storage* m_Storage;
		StorageReference m_CloudRootReference;
		const std::string m_RootString = "Root";
		const std::string m_LauncherString = "SWGLauncher.zip";
		StorageError m_LastStorageError;
		std::string m_LastStorageErrorString;
		static const char* kStorageUrl;
	};

	//EXPIMP_TEMPLATE FirebaseLib_API std::vector<FirebaseManager>;

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
			respond(manager->GetLastAuthErrorString().c_str());
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
		FirebaseLib_API void FirebaseManager_DownloadGameUpdate(FirebaseManager* manager, int version)
		{
			manager->DownloadGameUpdate(version);
		}
		FirebaseLib_API void FirebaseManager_DownloadGame(FirebaseManager* manager)
		{
			manager->DownloadGame();
		}
		FirebaseLib_API int FirebaseManager_NumberOfGameZips(FirebaseManager* manager)
		{
			return manager->NumberOfGameZips();
		}
		FirebaseLib_API void FirebaseManager_LogLauncher(FirebaseManager* manager, const char* log)
		{
			return manager->LogLauncher(log);
		}
	}
}

