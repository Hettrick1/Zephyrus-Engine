#include "pch.h"
#include "Timer.h"
#include <thread>

using namespace std::chrono;

float Timer::deltaTime = 0.0f;
unsigned int Timer::mFPS = 0;
unsigned int Timer::mFrameCount = 0;
unsigned int Timer::mLastFPSUpdate = 0;
unsigned int Timer::mFrameTime = 0;
float Timer::mDtMsDebug = 0.0f;
high_resolution_clock::time_point Timer::mFrameStart = high_resolution_clock::now();
high_resolution_clock::time_point Timer::mLastFrame = high_resolution_clock::now();

unsigned int Timer::ComputeDeltaTime()
{
    auto now = high_resolution_clock::now();
    auto dt = duration<double>(now - mLastFrame).count();
    mLastFrame = now;

    deltaTime = static_cast<float>(std::min(dt, MAX_DT / 1000.0));

    ++mFrameCount;

    double diff = duration_cast<milliseconds>(now.time_since_epoch()).count() - mLastFPSUpdate;

    if (diff >= 100 && diff <= 110)
    {
        mDtMsDebug = deltaTime * 1000;
    }

    if (diff >= 1000)
    {
        mFPS = mFrameCount;
        mFrameCount = 0;
        mLastFPSUpdate = duration_cast<milliseconds>(now.time_since_epoch()).count();
    }

    return static_cast<unsigned int>(dt * 1000.0);
}

void Timer::DelayTime()
{
    using dsec = duration<double>;
    
    auto frameDuration = duration_cast<high_resolution_clock::duration>(dsec{ 1.0 / MAX_FPS });

    static auto m_EndFrame = high_resolution_clock::now() + frameDuration;
    
    std::this_thread::sleep_until(m_EndFrame);
    
    m_EndFrame += frameDuration;
    
    auto now = high_resolution_clock::now();
    if (m_EndFrame < now)
    {
        m_EndFrame = now + frameDuration;
    }
}
