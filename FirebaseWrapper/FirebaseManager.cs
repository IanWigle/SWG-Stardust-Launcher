﻿using System.Runtime.InteropServices;

namespace SWGLauncher
{ 
    delegate void ResponseDelegate(string s);

    public enum AccountType
    {
        Invalid = 0,
        Anonymous,
        VerifiedEmail
    }


    /// <summary>
    /// Wrapper class to make it easier to call Firebase DLL Calls
    /// </summary>
    public class FirebaseManager
    {
        public FirebaseManager() { CreateManager(); }
        private void CreateManager() 
        {
            manager = FirebaseManager_Create(); 
        }
        public void DeleteManager()
        {
            FirebaseManager_Delete(manager);
        }
        public void SetEmail(string email)
        {
            FirebaseManager_SetEmail(manager, email);
        }
        public void SetPassword(string password)
        {
            FirebaseManager_SetPassword(manager, password);
        }
        public bool Register()
        {
            return FirebaseManager_Register(manager);
        }
        public string GetPassword()
        {
            string password = "";
            FirebaseManager_GetPassword(manager, s => password = s);
            return password;
        }
        public string GetEmail()
        {
            string password = "";
            FirebaseManager_GetEmail(manager, s => password = s);
            return password;
        }
        public void SignOut()
        {
            FirebaseManager_SignOut(manager);
        }

        public bool SignedIn()
        {
            return FirebaseManager_StillSignedIn(manager);
        }

        public void SignInAnon()
        {
            FirebaseManager_SignInAnon(manager);
        }

        public string GetClientVersion()
        {
            string version = "";
            FirebaseManager_GetClientVersion(manager, s => version = s);
            return version;
        }

        public AccountType GetAccountType()
        {
            switch(FirebaseManager_GetAccountType(manager))
            {
                case 1:
                    {
                        return AccountType.Anonymous;
                    }
                case 2:
                    {
                        return AccountType.VerifiedEmail;
                    }
                default:
                    {
                        return AccountType.Invalid;
                    }
            }
        }

        public void DownloadClient()
        {
            FirebaseManager_DownloadSWGLauncher(manager);
        }

        public string GetLastAuthErrorString()
        {
            string error = "";
            FirebaseManager_GetLastAuthErrorString(manager, s =>  error = s);
            return error;
        }

        public bool Login()
        {
            return FirebaseManager_Login(manager);
        }

        // refer to types.h in firebase::auth::AuthError to translate the int to the enum
        public int GetLastAuthError()
        {
            return FirebaseManager_GetAuthError(manager);
        }

        public void SendPasswordReset(string email)
        {
            FirebaseManager_SendPasswordResetEmail(manager, email);
        }

        public void SetDisplayName(string name)
        {
            FirebaseManager_SetDisplayName(manager, name);
        }

        public string GetDisplayName()
        {
            string name = "";
            FirebaseManager_GetDisplayName(manager, s =>  name = s);
            return name;
        }

        public string GetGameVersion()
        {
            string version = "";
            FirebaseManager_GetGameVersion(manager, s => version = s);
            return version;
        }

        public void DownloadGameUpdate(int version)
        {
            FirebaseManager_DownloadGameUpdate(manager, version);
        }

        public void DownloadGame()
        {
            FirebaseManager_DownloadGame(manager);
        }

        public int GetNumberOfGameFiles()
        {
            return FirebaseManager_NumberOfGameZips(manager);
        }

        public void LogLauncher(string log)
        {
            FirebaseManager_LogLauncher(manager, log);
        }

        public void LogUpdater(string log)
        {
            FirebaseManager_LogUpdater(manager, log);
        }

        public int GetLastStorageError()
        {
            return FirebaseManager_GetLastStorageError(manager);
        }

        public string GetLastStorageErrorString()
        {
            string error = "";
            FirebaseManager_GetLastStorageErrorString(manager, s => error = s);
            return error;
        }

        public void DownloadNewUpdater()
        {
            FirebaseManager_DownLoadNewUpdater(manager);
        }

        public string GetUpdaterVersion()
        {
            string version = "";
            FirebaseManager_GetUpdaterVersion(manager, s => version = s);
            return version;
        }

        public bool GetUnderMaintenance()
        {
            return FirebaseManager_UnderMaintenance(manager);
        }

        public bool DoesUserNameExist(string username)
        {
            return FirebaseManager_DoesUserNameExist(manager, username);
        }

        public void ChangeEmail(string newEmail)
        {
            FirebaseManager_UpdateEmail(manager, newEmail);
        }

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr FirebaseManager_Create();

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_Delete(IntPtr value);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_SetEmail(IntPtr value, string email);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_SetPassword(IntPtr value, string str);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool FirebaseManager_Register(IntPtr value);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        private static extern void FirebaseManager_GetPassword(IntPtr manager, ResponseDelegate response);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        private static extern void FirebaseManager_GetEmail(IntPtr manager, ResponseDelegate response);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_SignOut(IntPtr value);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool FirebaseManager_StillSignedIn(IntPtr value);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_SignInAnon(IntPtr value);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        private static extern void FirebaseManager_GetClientVersion(IntPtr manager, ResponseDelegate response);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern int FirebaseManager_GetAccountType(IntPtr value);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_DownloadSWGLauncher(IntPtr value);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_GetLastAuthErrorString(IntPtr value, ResponseDelegate response);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool FirebaseManager_Login(IntPtr value);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern int FirebaseManager_GetAuthError(IntPtr value);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_SendPasswordResetEmail(IntPtr value, string str);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_SetDisplayName(IntPtr value, string str);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_GetDisplayName(IntPtr value, ResponseDelegate response);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_GetGameVersion(IntPtr value, ResponseDelegate response);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_DownloadGameUpdate(IntPtr value, int version);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_DownloadGame(IntPtr value);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern int FirebaseManager_NumberOfGameZips(IntPtr value);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern int FirebaseManager_LogLauncher(IntPtr value, string log);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern int FirebaseManager_LogUpdater(IntPtr value, string log);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_GetLastStorageErrorString(IntPtr value, ResponseDelegate response);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern int FirebaseManager_GetLastStorageError(IntPtr value);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_DownLoadNewUpdater(IntPtr value);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern int FirebaseManager_GetUpdaterVersion(IntPtr value, ResponseDelegate response);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool FirebaseManager_UnderMaintenance(IntPtr value);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool FirebaseManager_DoesUserNameExist(IntPtr value, string username);

        [DllImport(@"FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_UpdateEmail(IntPtr value, string newEmail);

        private static IntPtr manager;
    }
}
