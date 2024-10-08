using System.Diagnostics;
using SWGLauncher.Forms;

namespace SWGLauncher
{
    internal static class Program
    {
        static FirebaseManager firebase = null;
        static AudioManager audioManager = null;
        static ProgramSettings programSettings = null;

        // Modules
        static public FirebaseManager GetFirebaseManager() => firebase;
        static public AudioManager GetAudioManager() => audioManager;

        // Settings
        static public ProgramSettings GetSettings() => programSettings;
        public static int FirstLaunch { get; private set; }

        // Version Numbers
        public const double Launcherversion = 2.1;
        public static double UpdaterVersion { get; set; }
        public static double GameVersion { get; private set; }
        public static double FirebaseLauncherVersion { get; private set; }
        public static double FirebaseGameVersion { get; private set; }
        public static double FirebaseUpdaterVersion { get; private set; }

        // Status
        public static bool DownloadingGame = false;

        // Server IPs
        public static List<IPData> IPAddresses { get; set; }
        public static IPData LastIP { get; set; }

        // Audio
        public const string AudioFileName = "Star-Dust.wav";

        /// <summary>
        ///  The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            if(args.Contains("FromUpdater"))
                Directory.SetCurrentDirectory(@"..\");

            //if(File.Exists(@".\google-services.json"))
            //{
            //    FileStream file = File.Create(@".\google-services.json");
            //    string serv = SWGLauncher.Properties.Resources.google_services;
            //    file.Write(Encoding.ASCII.GetBytes(serv));
            //}

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.SetHighDpiMode(HighDpiMode.SystemAware);
            ApplicationConfiguration.Initialize();

            if (!Directory.Exists(@".\Logs"))
                Directory.CreateDirectory(@".\Logs");

            firebase = new FirebaseManager();
            firebase.SignInAnon();
            FirebaseLauncherVersion = Convert.ToDouble(firebase.GetClientVersion());
            FirebaseGameVersion = double.Parse(firebase.GetGameVersion());
            FirebaseUpdaterVersion = double.Parse(firebase.GetUpdaterVersion());

            firebase.LogLauncher("Program : Firebase loaded succesfuly");

            IPAddresses = new List<IPData>();

            LoadSettings();

            Debug.WriteLine($"Current clientside client version is {Launcherversion}");
            Debug.WriteLine($"The most up to date on firebase is {FirebaseLauncherVersion}");

            audioManager = new AudioManager(@$".\Resources\{AudioFileName}");
            firebase.LogLauncher($"Program : Audio Manager loaded with {AudioFileName}");

            // Main Loop
            Application.Run(new MainForm());
            firebase.LogLauncher($"Program : Shutting down launcher.");

            if (!DownloadingGame) FirstLaunch++;
            SaveSettings();
            firebase.LogLauncher($"Program : Firebase unloaded.");
            firebase.DeleteManager();
        }

        static void LoadSettings()
        {
            if(File.Exists(@".\LauncherSettings.cfg"))
            {
                firebase.LogLauncher("Program : LauncherSettings found, loading them into launcher.");

                bool enableSound = false;
                bool randomBackgrounds = false;
                bool rememberUser = false;
                bool enableDebugString = false;
                int specImage = 0;
                string lastEmail = "";

                string[] launcher_prop_lines = File.ReadAllLines(@".\LauncherSettings.cfg");
                string[] user_prop_lines = File.ReadAllLines(@".\user.cfg");

                if(launcher_prop_lines.Length < 0)
                {
                    firebase.LogLauncher("Program : LauncherSettings file was empty, resorting to default settings.");
                    programSettings = new ProgramSettings();
                    return;
                }
                else if(user_prop_lines.Length < 0)
                {
                    firebase.LogLauncher("Program: user.cfg file was empty, May cause problems.");
                }

                try
                {
                    enableSound = Convert.ToBoolean(launcher_prop_lines[0].Split(':')[1]);
                    randomBackgrounds = Convert.ToBoolean(launcher_prop_lines[1].Split(':')[1]);
                    specImage = Convert.ToInt32(launcher_prop_lines[2].Split(':')[1]);
                    rememberUser = Convert.ToBoolean(launcher_prop_lines[3].Split(':')[1]);
                    lastEmail = (launcher_prop_lines[4].Split(':').Length > 1) ? Convert.ToString(launcher_prop_lines[4].Split(':')[1]) : "";
                    GameVersion = Convert.ToDouble(launcher_prop_lines[5].Split(':')[1]);
                    FirstLaunch = Convert.ToInt32(launcher_prop_lines[6].Split(':')[1]);
                    UpdaterVersion = Convert.ToDouble(launcher_prop_lines[7].Split(':')[1]);
                }
                catch(Exception ex)
                {
                    firebase.LogLauncher("Program : There was a problem with the LauncherSettings, loading default settings into launcher.");
                    firebase.LogLauncher($"Program : {ex.Message}");

                    GameVersion = 1;
                    FirstLaunch = 0;
                    UpdaterVersion = 0.1;
                }

                try
                {
                    enableDebugString = Convert.ToBoolean(user_prop_lines[23].Split('=')[1]);
                }
                catch(Exception ex)
                {
                    firebase.LogLauncher("Program : There was a problem with the user.cfg, loading default settings into launcher.");
                    firebase.LogLauncher($"Program : {ex.Message}");
                }

                programSettings = new ProgramSettings(enableSound, randomBackgrounds, specImage, rememberUser, lastEmail, enableDebugString);
            }
            else
            {
                firebase.LogLauncher("Program : LauncherSettings was not found, loading default settings into launcher.");
                programSettings = new ProgramSettings();
                GameVersion = 1;
                FirstLaunch = 0;
                UpdaterVersion = 0.1;
            }

            if(File.Exists(@".\ServerList.cfg"))
            {
                string[] servers = File.ReadAllLines(@".\ServerList.cfg");

                foreach(string server in servers)
                {
                    string[] serverBits = server.Split(":");
                    if (serverBits.Length < 3) continue;
                    else if(serverBits.Length > 3)
                    {
                        LastIP = new IPData(serverBits[2], serverBits[1], int.Parse(serverBits[3]));
                        //IPAddresses.Add(new IPData(serverBits[2], serverBits[1], int.Parse(serverBits[3])));
                    }
                    else
                        IPAddresses.Add(new IPData(serverBits[1], serverBits[0], int.Parse(serverBits[2])));
                }
            }
            //else if (File.Exists(@".\login.cfg"))
            //{
            //    string[] lines = File.ReadAllLines(@".\login.cfg");

            //    // We're looking for line 9 (8 in code)
            //    string lastLine = lines[lines.Length - 1];

            //    // Seperate ip numbers from text property by splitting string by the '=' symbol
            //    string ip = lastLine.Split('=')[1];

            //    IPAddresses.Add(new IPData(ip, "DS-II"));
            //}
        }

        public static void SaveSettings()
        {
            firebase.LogLauncher("Program : Exporting Launcher settings to LauncherSettings.cfg");
            List<string> lines = new List<string>();

            lines.Add($"EnableSound:{programSettings.EnableSound}");
            lines.Add($"RandomizeBackgrounds:{programSettings.RandomizeBackgrounds}");
            lines.Add($"SpecificImage:{programSettings.SpecificImage}");
            lines.Add($"RememberLastUser:{programSettings.RememberLastUser}");
            lines.Add($"LastEmail:{programSettings.LastEmail}");
            lines.Add($"GameVersion:{GameVersion}");
            lines.Add($"FirstLaunch:{FirstLaunch}");
            lines.Add($"Updater:{UpdaterVersion}");

            File.WriteAllLines(@".\LauncherSettings.cfg", lines.ToArray());

            if(IPAddresses.Count > 0)
            {
                List<string> literalSTrings = new List<string>();
                foreach(IPData iPData in IPAddresses)
                {
                    literalSTrings.Add(iPData.ToString());
                }
                literalSTrings.Add($"LastIP:{LastIP.ToString()}");

                File.WriteAllLines(@".\ServerList.cfg",literalSTrings.ToArray());
            }
        }
    }
}