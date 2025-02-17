#pragma once
#include <functional>

class TDoOnce
{
private:
	bool bHaseExcuted = false;

public:
	bool Execute(std::function<void()> Function)
	{
		if (bHaseExcuted)
			return false;

		if (Function)
			Function();

		bHaseExcuted = true;
		return true;
	}
	void Reset()
	{
		bHaseExcuted = false;
	}
};