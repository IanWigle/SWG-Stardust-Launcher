using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Diagnostics;

namespace SWGLauncher
{
    internal static class Program
    {
        static FirebaseManager firebase = null;

        static public FirebaseManager GetFirebaseManager() => firebase;

        public const int Launcherversion = 2;
        public static int FirebaseLauncherVersion { get; private set; }

        /// <summary>
        ///  The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            // To customize application configuration such as set high DPI settings or default font,
            // see https://aka.ms/applicationconfiguration.
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            ApplicationConfiguration.Initialize();

            firebase = new FirebaseManager();
            firebase.SignInAnon();
            FirebaseLauncherVersion = int.Parse(firebase.GetClientVersion());
            Debug.WriteLine($"Current clientside client version is {Launcherversion}");
            Debug.WriteLine($"The most up to date on firebase is {FirebaseLauncherVersion}");
            
            Application.Run(new Form1());

            if(firebase.GetAccountType() == AccountType.Anonymous)
            {
                firebase.DeleteCurrentAccount();
            }
            else
            {
                firebase.SignOut();
            }
            firebase.DeleteManager();
        }
    }
}