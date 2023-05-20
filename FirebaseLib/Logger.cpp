#include "pch.h"


using namespace std;

FirebaseLib::Logger::Logger()
{
	string launch = LogLauncherFileName + getCurrentDateTime("date") + LogExt;
	string firebase = LogFirebaseFileName + getCurrentDateTime("date") + LogExt;
	freopen(launch.c_str(),"w", stderr);
	freopen(firebase.c_str(),"w",stdout);
}

FirebaseLib::Logger::~Logger()
{
}

void FirebaseLib::Logger::LogLauncher(const char* text, ...)
{
	cerr << getCurrentDateTime("now") << " : " << text << endl;
}

void FirebaseLib::Logger::LogFirebase(const char* text, ...)
{
	cout << getCurrentDateTime("now") << " : " << text << endl;
}
