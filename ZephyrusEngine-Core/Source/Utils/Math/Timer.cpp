#include "pch.h"
#include "Timer.h"
#include <thread>

using namespace std::chrono;

float Timer::deltaTime = 0.0f;
unsigned int Timer::mFPS = 0;
unsigned int Timer::mFrameCount = 0;
unsigned int Timer::mLastFPSUpdate = 0;
unsigned int Timer::mFrameTime = 0;
high_resolution_clock::time_point Timer::mFrameStart = high_resolution_clock::now();
high_resolution_clock::time_point Timer::mLastFrame = high_resolution_clock::now();

unsigned int Timer::ComputeDeltaTime()
{
    mFrameStart = high_resolution_clock::now();
    auto dt_ms = duration_cast<milliseconds>(mFrameStart - mLastFrame).count();
    mLastFrame = mFrameStart;

    dt_ms = std::min(dt_ms, static_cast<long long>(MAX_DT));
    deltaTime = dt_ms / 1000.0f;

    ++mFrameCount;

    if (duration_cast<milliseconds>(mFrameStart.time_since_epoch()).count() - mLastFPSUpdate >= 1000)
    {
        mFPS = mFrameCount;
        mFrameCount = 0;
        mLastFPSUpdate = duration_cast<milliseconds>(mFrameStart.time_since_epoch()).count();
    }

    return static_cast<unsigned int>(dt_ms);
}

void Timer::DelayTime()
{
    auto frameTime = duration_cast<milliseconds>(high_resolution_clock::now() - mFrameStart).count();
    if (frameTime < FRAME_DELAY)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_DELAY - frameTime));
    }
}
