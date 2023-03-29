#pragma once
#include <chrono>
#include <iostream>

class PerformaceTimer
{
public:
    PerformaceTimer();
    ~PerformaceTimer();

private:
    std::chrono::high_resolution_clock::time_point myStart;
    std::chrono::duration<float> myDuration;
};

inline PerformaceTimer::PerformaceTimer()
{
    myStart = std::chrono::high_resolution_clock::now();
}

inline PerformaceTimer::~PerformaceTimer()
{
    myDuration = std::chrono::high_resolution_clock::now() - myStart;

    float ms = myDuration.count() * 1000.0f;
    std::cout << "\nTime took " << ms << "ms" << std::endl;
}
