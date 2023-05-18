#pragma once

class DownloadListener : public firebase::storage::Listener
{
public:
	virtual void OnProgress(firebase::storage::Controller* controller);

	virtual void OnPaused(firebase::storage::Controller* controller);

	inline void SetStorageRef(firebase::storage::Storage* ref) { m_StorageRef = ref; }
private:
	firebase::storage::Storage* m_StorageRef;
};