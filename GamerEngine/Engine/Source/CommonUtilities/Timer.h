#pragma once
#include <array>
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

        int GetFPS();
        float GetDeltaTime() const;
        double GetTotalTime() const;
        int GetElapsedFrames() const;

    private:

        float CalculateFPS() const;
        int lastFrameIndex = 0;
        std::array<float, 50> myFrameValues{};
        int myFramesPerSecond = 0;
        unsigned int myElapsedFrames = 0;
        float myDeltaTime;
        double myTotalTimeElapsed = 0;
        std::chrono::steady_clock::time_point myStartTime;
    };
}