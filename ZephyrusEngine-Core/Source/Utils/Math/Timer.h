#pragma once

#include <algorithm>
#include <chrono>
#include <algorithm>

class Timer
{
public:
	Timer() = default;
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

	static unsigned int ComputeDeltaTime();
	static void DelayTime();

	static float deltaTime;
	static float mDtMsDebug; // only for showing in debug, do not use it in critical functions
	static unsigned int mFPS;

private:
	const static unsigned int MAX_FPS = 60;
	const static unsigned int FRAME_DELAY = 1000 / MAX_FPS;
	//Maximum Delta Time
	const static unsigned int MAX_DT = 50;

	static unsigned int mFrameCount;
	static unsigned int mLastFPSUpdate;

	//Time when current frame started
	static std::chrono::high_resolution_clock::time_point mFrameStart;
	//Last frame start time 
	static std::chrono::high_resolution_clock::time_point mLastFrame;
	//Time needed to run the loop 
	static unsigned int mFrameTime;
};

