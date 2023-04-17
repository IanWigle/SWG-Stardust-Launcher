#pragma once

namespace FirebaseLib
{
	class FirebaseLib_API FirebaseManager : public Manager
	{
	public:
		FirebaseManager(const std::string& email, const std::string& password);
		explicit FirebaseManager();

		virtual void Destroy() override;

	private:

	};
}

