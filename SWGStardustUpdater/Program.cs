// See https://aka.ms/new-console-template for more information
//Console.WriteLine("Hello, World!");
//Console.ReadLine();
using SWGLauncher;
using System;
using System.IO.Compression;
using System.Diagnostics;

partial class Program
{
    static FirebaseManager manager = null;

    static void Main(string[] args)
    {
        try
        {
            // Initialize the firebase manager
            manager = new FirebaseManager();
            manager.SignInAnon();

            if(args.Contains("FromClient"))
                Directory.SetCurrentDirectory(@".\Updater\");

            // Hard coded version for updater.
            const double Version = 0.1;

            Console.WriteLine($"Welcome to the SWG Stardust Updater! Version {Version}.");

            if (args.Contains("Launcher"))
                DownloadLauncher();
            else if (args.Contains("Update"))
                DownloadUpdate();
            else if (args.Contains("Game"))
                DownloadGame();
            else
            {
                Console.WriteLine("ERROR : Missing argument to determine what to download.");
            }

            Console.WriteLine("Press enter to close.");

            // Wait for input from user so they control when this program closes. 
            // Just press ENTER
            Console.ReadLine();

            if (Directory.Exists(@".\NewFiles"))
                Directory.Delete(@".\NewFiles");    

            manager.DeleteManager();
            Process process = new Process();
            process.StartInfo.FileName = @"..\SWGLauncher.exe";
            process.StartInfo.ArgumentList.Add("FromUpdater");
            process.Start();
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
            if (manager != null) Console.WriteLine(manager.GetLastError());
            Console.ReadLine();
            if (manager != null) manager.DeleteManager();
        }
    }

    static void DownloadLauncher()
    {
        const string NewLauncher = @".\SWGLauncher.zip";

        // If there is a local version of the launcher files already, delete it. It's likely old.
        if (File.Exists(NewLauncher))
        {
            File.Delete(NewLauncher);
        }

        Console.WriteLine("Downloading the new client files.");
        manager.DownloadClient();

        // Check if the files were successfully downloaded by checking if the file exists.
        if (File.Exists(NewLauncher))
        {
            Console.WriteLine("Client was downloaded! Replacing old files.");
            Console.WriteLine("Do not touch any client files!");
            ZipFile.ExtractToDirectory(NewLauncher, @"..\", true);
        }
        else
        {
            Console.WriteLine("ERROR: Unable to open the zip file containing new launcher. Aborting!");
            return;
        }

        Console.WriteLine("New client files were replaced!");

        File.Delete(NewLauncher);
    }

    static void DownloadUpdate()
    {
        const string NewUpdate = @".\StardustUpdate.zip";
        int CurrentGameVersion = 1;
        int CurrentFirebaseGameVersion = int.Parse(manager.GetGameVersion());

        if (File.Exists(@"..\LauncherSettings.cfg"))
        {
            string[] lines = File.ReadAllLines(@"..\LauncherSettings.cfg");

            lines[5] = $"GameVersion:{manager.GetGameVersion()}";
            CurrentGameVersion = int.Parse(lines[5].Split(':')[1]);
        }

        if(CurrentGameVersion >= CurrentFirebaseGameVersion)
        {
            Console.WriteLine("ERROR: Somehow the game is either already updated, or newever than whats on the cloud.");
            return;
        }

        {
            string[] files = Directory.GetFiles(Directory.GetCurrentDirectory());
            foreach(string file in files)
            {
                if (files.Contains(NewUpdate))
                    File.Delete($@".\{file}");
            }
        }

        Console.WriteLine("Downloading new update files");
        do
        {
            if ((CurrentGameVersion + 1) > CurrentFirebaseGameVersion) break;
            manager.DownloadGameUpdate(CurrentGameVersion + 1);
            CurrentGameVersion++;
        } while (CurrentGameVersion != CurrentFirebaseGameVersion);


        if(File.Exists(NewUpdate))
        {
            Console.WriteLine("New Game Update files downloaded! Replacing old files.");
            Console.WriteLine("Do not touch any game files!");
            ZipFile.ExtractToDirectory(NewUpdate, @"..\", true);
        }
        else
        {
            Console.WriteLine("ERROR : Unable to open the zip file containing new game files. Aborting!");
            return;
        }

        Console.WriteLine("Game Update patched!");

        File.Delete(NewUpdate);

        // Update the launcher cfg
        if(File.Exists(@"..\LauncherSettings.cfg"))
        {
            string[] lines = File.ReadAllLines(@"..\LauncherSettings.cfg");

            lines[5] = $"GameVersion:{manager.GetGameVersion()}";
            File.WriteAllLines(@"..\LauncherSettings.cfg", lines.ToArray());
        }
    }

    static void DownloadGame()
    {
        const string NewGame = @".\StardustGameFiles";
        int NumFiles = manager.GetNumberOfGameFiles();

        if (File.Exists(NewGame))
        {
            File.Delete(NewGame);
        }

        for (int i = 1; i < NumFiles + 1; i++)
        {
            if (File.Exists(@$"{NewGame}{i}.zip"))
                File.Delete(@$"{NewGame}{i}.zip");
        }

        Console.WriteLine("Downloading Game files");
        manager.DownloadGame();

        if (manager.GetLastError() == "")
        {
            Console.WriteLine("New Game Update files downloaded! Replacing old files.");
            Console.WriteLine("Do not touch any game files!");

            for (int i = 1; i < NumFiles + 1; i++)
            {
                ZipFile.ExtractToDirectory($"{NewGame}{i}.zip", @"..\", true);
            }
        }
        else
        {
            Console.WriteLine("ERROR : Unable to open the zip file containing new game files. Aborting!");
            return;
        }

        Console.WriteLine("Game Update patched!");

        for (int i = 1; i < NumFiles + 1; i++)
        {
            if (File.Exists(@$"{NewGame}{i}.zip"))
                File.Delete(@$"{NewGame}{i}.zip");
        }

        // Update the launcher cfg
        if (File.Exists(@"..\LauncherSettings.cfg"))
        {
            string[] lines = File.ReadAllLines(@"..\LauncherSettings.cfg");

            lines[5] = $"GameVersion:{manager.GetGameVersion()}";
            File.WriteAllLines(@"..\LauncherSettings.cfg", lines.ToArray());
        }
    }
}


