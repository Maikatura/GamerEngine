#pragma once
#include <chrono>

namespace CommonUtilities
{
    class Timer
    {
    public:
        Timer();
        Timer(const Timer& aTimer) = delete;
        Timer& operator=(const Timer& aTimer) = delete;

        void Update();

        float GetDeltaTime() const;
        double GetTotalTime() const;
        int GetElapsedFrames() const;

    private:
        unsigned int myElapsedFrames = 0;
        float myDeltaTime;
        double myTotalTimeElapsed = 0;
        std::chrono::steady_clock::time_point myStartTime;
    };
}