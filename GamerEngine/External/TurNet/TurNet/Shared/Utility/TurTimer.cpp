#include "TurTimer.h"

TurNet::TurTimer::TurTimer(): myDeltaTime(0)
{
	myStartTime = std::chrono::high_resolution_clock::now();
}

void TurNet::TurTimer::Update()
{
    std::chrono::steady_clock::time_point clockNow = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time = clockNow - myStartTime;
    myStartTime = clockNow;
    myDeltaTime = static_cast<float>(time.count()) / 1000.0f;
}

float TurNet::TurTimer::GetDeltaTime() const
{
    return myDeltaTime;
}