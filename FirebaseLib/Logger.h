#pragma once

namespace FirebaseLib
{
	class Logger
	{
	public:
		Logger();
		~Logger();

		void LogLauncher(const char* text, ...);
		void LogFirebase(const char* text, ...);

		static inline std::string getCurrentDateTime(std::string s) {
			time_t now = time(0);
			struct tm  tstruct;
			char  buf[80];
			tstruct = *localtime(&now);
			if (s == "now")
				strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
			else if (s == "date")
				strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
			return std::string(buf);
		};

	private:
		const std::string LogLauncherFileName = "Logs\\LauncherLog";
		const std::string LogFirebaseFileName = "Logs\\FirebaseLog";
		const std::string LogExt = ".txt";		
	};
}