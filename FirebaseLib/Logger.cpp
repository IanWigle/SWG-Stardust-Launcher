#include "pch.h"
#include <fstream>

using namespace std;

FirebaseLib::Logger::Logger()
{
}

FirebaseLib::Logger::~Logger()
{
	// Handle launcher.txt
	if (!LauncherLog.size() == 0)
	{
		ofstream launcherLogFile(LogLauncherFileName + getCurrentDateTime("date") + LogExt);
		for (string line : LauncherLog)
		{
			launcherLogFile << line;
		}
		launcherLogFile.close();
	}
	
	if (!FirebaseLog.size() == 0)
	{
		// Handle firebase.txt
		ofstream firebaseLogFile(LogFirebaseFileName + getCurrentDateTime("date") + LogExt);
		for (string line : FirebaseLog)
		{
			firebaseLogFile << line;
		}
		firebaseLogFile.close();
	}

	if (!UpdaterLog.size() == 0)
	{
		ofstream updaterLogFile(LogUpdaterFileName + getCurrentDateTime("date") + LogExt);
		for (string line : UpdaterLog)
		{
			updaterLogFile << line;
		}
		updaterLogFile.close();
	}
}

void FirebaseLib::Logger::LogLauncher(const char* text, ...)
{
	LauncherLog.push_back(string(getCurrentDateTime("now") + " : " + text + "\n"));
}

void FirebaseLib::Logger::LogFirebase(const char* text, ...)
{
	FirebaseLog.push_back(string(getCurrentDateTime("now") + " : " + text + "\n"));
}

void FirebaseLib::Logger::LogUpdater(const char* text, ...)
{
	UpdaterLog.push_back(string(getCurrentDateTime("now") + " : " + text + "\n"));
}
