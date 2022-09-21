#pragma once
#include <Timer.h>

class Time
{
public:
	static float GetDeltaTime()
	{
		return myTimer.GetDeltaTime();
	}

	static int GetFPS()
	{
		return myTimer.GetFPS();
	}

	static void Update()
	{
		myTimer.Update();

	}

private:

	


	inline static CommonUtilities::Timer myTimer;
};