#include "pch.h"
#include "DownloadListener.h"

void DownloadListener::OnProgress(firebase::storage::Controller* controller)
{
	double progress = (100.0 * controller->bytes_transferred()) / controller->total_byte_count();
	std::string sprogress = "Progress : " + std::to_string(progress);
	LogMessage(sprogress.c_str());
}

void DownloadListener::OnPaused(firebase::storage::Controller* controller)
{
	//controller->Resume();
}
