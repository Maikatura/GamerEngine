#pragma once
#include <array>
#include <chrono>

namespace TurNet
{
    class TurTimer
    {
    public:
        TurTimer();
        TurTimer(const TurTimer& aTimer) = delete;
        TurTimer& operator=(const TurTimer& aTimer) = delete;

        void Update();
        float GetDeltaTime() const;
       

    private:
        float myDeltaTime;
        std::chrono::steady_clock::time_point myStartTime;
    };
}