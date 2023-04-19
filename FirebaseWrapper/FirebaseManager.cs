using System;
using System.Runtime.InteropServices;

namespace SWGLauncher
{ 
    delegate void ResponseDelegate(string s);

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
        public void Register()
        {
            FirebaseManager_Register(manager);
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
            string ver = "";
            FirebaseManager_GetClientVersion(manager, s => ver = s);
            return ver;
        }

        [DllImport("FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr FirebaseManager_Create();

        [DllImport("FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_Delete(IntPtr value);

        [DllImport("FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_SetEmail(IntPtr value, string email);

        [DllImport("FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_SetPassword(IntPtr value, string str);

        [DllImport("FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_Register(IntPtr value);

        [DllImport("FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        private static extern void FirebaseManager_GetPassword(IntPtr manager, ResponseDelegate response);

        [DllImport("FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        private static extern void FirebaseManager_GetEmail(IntPtr manager, ResponseDelegate response);

        [DllImport("FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_SignOut(IntPtr value);

        [DllImport("FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool FirebaseManager_StillSignedIn(IntPtr value);

        [DllImport("FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        private static extern void FirebaseManager_SignInAnon(IntPtr value);

        [DllImport("FirebaseLib.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        private static extern int FirebaseManager_GetClientVersion(IntPtr manager, ResponseDelegate response);

        private static IntPtr manager;
    }
}
