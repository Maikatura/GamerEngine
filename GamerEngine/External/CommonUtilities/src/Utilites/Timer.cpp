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
    myDeltaTime = static_cast<float>(time.count()) / 1000.0f;
    myTotalTimeElapsed += static_cast<double>(myDeltaTime);
    ++myElapsedFrames;

    myFrameValues[lastFrameIndex] = myDeltaTime;
    lastFrameIndex = (lastFrameIndex + 1) % myFrameValues.size();

    myFramesPerSecond = static_cast<int>(CalculateFPS());
}

float CommonUtilities::Timer::CalculateFPS() const
{
    float total = 0.0f;
    for(const float aDelta : myFrameValues)
    {
        total += aDelta;
    }

    return static_cast<float>(myFrameValues.size()) / total;
}

int CommonUtilities::Timer::GetFPS()
{
    return myFramesPerSecond;
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
    return static_cast<int>(myElapsedFrames) - 1;
}

float CommonUtilities::Timer::GetFrameTime()
{
	return (myFramesPerSecond != 0) ? 1000.0f / static_cast<float>(myFramesPerSecond) : 0.0f;
}
