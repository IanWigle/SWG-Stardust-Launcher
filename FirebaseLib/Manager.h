#pragma once

namespace FirebaseLib
{
	class FirebaseLib_API Manager
	{
	public:
		virtual void Destroy()
		{
			delete this;
		}

		virtual const char* GetManagerName() const = 0;
	};
}

