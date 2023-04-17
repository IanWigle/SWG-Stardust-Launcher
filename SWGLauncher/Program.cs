using System;
using System.Runtime.InteropServices;
using System.Text;

namespace SWGLauncher
{
    internal static class Program
    {
        static FirebaseManager firebase = null;

        static public FirebaseManager GetFirebaseManager() => firebase;

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

            // Sign out if session is still running
            if (firebase.SignedIn() == true)
                firebase.SignOut();

            Application.Run(new Form1());

            firebase.SignOut();
            firebase.DeleteManager();
        }
    }
}