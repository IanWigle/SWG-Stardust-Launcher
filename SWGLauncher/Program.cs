using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Diagnostics;
using System.Text.Json;

namespace SWGLauncher
{
    internal static class Program
    {
        static FirebaseManager firebase = null;
        static AudioManager audioManager = null;
        static ProgramSettings programSettings = null;

        static public FirebaseManager GetFirebaseManager() => firebase;
        static public AudioManager GetAudioManager() => audioManager;

        static public ProgramSettings GetSettings() => programSettings;

        public const int Launcherversion = 1;
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

            LoadSettings();

            audioManager = new AudioManager($"{Directory.GetCurrentDirectory()}\\Resources\\Star-Dust.wav");

            firebase = new FirebaseManager();
            firebase.SignInAnon();
            FirebaseLauncherVersion = int.Parse(firebase.GetClientVersion());
            Debug.WriteLine($"Current clientside client version is {Launcherversion}");
            Debug.WriteLine($"The most up to date on firebase is {FirebaseLauncherVersion}");

            // Main Loop
            Application.Run(new MainForm());

            SaveSettings();
            firebase.DeleteManager();
        }

        static void LoadSettings()
        {
            if(File.Exists(@".\LauncherSettings.cfg"))
            {
                bool enableSound = false;
                bool randomBackgrounds = false;
                bool rememberUser = false;
                int specImage = 0;
                string lastEmail = "";

                string[] lines = File.ReadAllLines(@".\LauncherSettings.cfg");

                if(!(lines.Length > 0))
                {
                    programSettings = new ProgramSettings();
                    return;
                }

                enableSound = Convert.ToBoolean(lines[0].Split(':')[1]);
                randomBackgrounds = Convert.ToBoolean(lines[1].Split(':')[1]);
                specImage = Convert.ToInt32(lines[2].Split(':')[1]);
                rememberUser = Convert.ToBoolean(lines[3].Split(':')[1]);
                lastEmail = (lines[4].Split(':').Length > 1) ? Convert.ToString(lines[4].Split(':')[1]) : ""; 

                programSettings = new ProgramSettings(enableSound, randomBackgrounds, specImage, rememberUser, lastEmail);
            }
            else
            {
                programSettings = new ProgramSettings();
            }
        }

        static void SaveSettings()
        {
            List<string> lines = new List<string>();

            lines.Add($"EnableSound:{programSettings.EnableSound}");
            lines.Add($"RandomizeBackgrounds:{programSettings.RandomizeBackgrounds}");
            lines.Add($"SpecificImage:{programSettings.SpecificImage}");
            lines.Add($"RememberLastUser:{programSettings.RememberLastUser}");
            lines.Add($"LastEmail:{programSettings.LastEmail}");

            File.WriteAllLines(@".\LauncherSettings.cfg", lines.ToArray());
        }
    }
}