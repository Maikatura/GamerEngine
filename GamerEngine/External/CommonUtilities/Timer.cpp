#include "pch.h"
#include "Timer.h"

CommonUtilities::Timer::Timer(): myDeltaTime(0)
{
	myStartTime = std::chrono::high_resolution_clock::now();
}

void CommonUtilities::Timer::Update()
{
    std::chrono::steady_clock::time_point clockNow = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time = clockNow - myStartTime;
    myStartTime = clockNow;
    myDeltaTime = static_cast<float>(time.count() / 1000);
    myTotalTimeElapsed += static_cast<double>(myDeltaTime);
    ++myElapsedFrames;
}

float CommonUtilities::Timer::GetDeltaTime() const
{
    return myDeltaTime;
}

double CommonUtilities::Timer::GetTotalTime() const
{
    return myTotalTimeElapsed;
}

int CommonUtilities::Timer::GetElapsedFrames() const
{
    return myElapsedFrames - 1;
}