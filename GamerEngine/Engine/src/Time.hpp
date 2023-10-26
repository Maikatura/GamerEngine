#pragma once
#include <Utilites/Timer.h>

class Time
{
public:

	

	static float GetDeltaTime()
	{
		return DeltaTime;
	}

	static int GetFPS()
	{
		return FPS;
	}

private:

	static void Update()
	{
		myTimer.Update();
		DeltaTime = myTimer.GetDeltaTime();
		FPS = myTimer.GetFPS();
		FrameTime = myTimer.GetFrameTime();
	}

	friend class GraphicsEngine;


	inline static int FPS;
	inline static float DeltaTime;
	inline static float FrameTime;


	inline static CommonUtilities::Timer myTimer;
};