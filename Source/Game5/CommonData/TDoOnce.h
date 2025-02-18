#pragma once
#include <functional>
#include <mutex>

class TDoOnce
{
private:
	bool bHasExecuted = false;
	std::mutex mx;

public:
	bool Execute(std::function<void()> Function)
	{
		std::lock_guard<std::mutex> Lock(mx);

		if (bHasExecuted)
			return false;

		Function();

		bHasExecuted = true;
		return true;
	}
	
	void Reset()
	{
		std::lock_guard<std::mutex> Lock(mx);

		bHasExecuted = false;
	}
};