// See https://aka.ms/new-console-template for more information
//Console.WriteLine("Hello, World!");
//Console.ReadLine();
using SWGLauncher;
using System;
using System.IO.Compression;
using System.Diagnostics;

FirebaseManager manager = null;

try
{
    // Initialize the firebase manager
    manager = new FirebaseManager();
    manager.SignInAnon();

    // Hard coded version for updater and where the swglauncher files will go.
    const double Version = 0.1;
    const string NewLauncher = @".\SWGLauncher.zip";

    Console.WriteLine($"Welcome to the SWG Stardust Updater! Version {Version}.");
    Console.WriteLine("Updating launcher files");

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
        ZipFile.ExtractToDirectory(NewLauncher, $"{Directory.GetCurrentDirectory()}..\\", true);        
    }
    else
    {
        Console.WriteLine("ERROR : Unable to open the zip file containing new launcher. Aborting!");
        return;
    }

    Console.WriteLine("New client files were replaced!");

    File.Delete(NewLauncher);

    Console.WriteLine("Press enter to close.");

    // Wait for input from user so they control when this program closes. 
    // Just press ENTER
    Console.ReadLine();

    manager.SignOut();
    manager.DeleteManager();
    Process.Start(@"..\SWGLauncher.exe");
}
catch(Exception e)
{
    Console.WriteLine(e);
    Console.ReadLine();
}

