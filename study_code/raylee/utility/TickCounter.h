#pragma once

#include <chrono>
#include <stdexcept>


class MyCounter
{
public:
	MyCounter()
		: mElapsedDur(std::chrono::duration<double, std::nano>())
	{
	}
	~MyCounter() = default;

	void start()
	{
		mStartClock = std::chrono::high_resolution_clock::now();
#if _DEBUG
		mStartFlag = true;
#endif
	}

	void end()
	{
#if _DEBUG
		if (!mStartFlag)
			throw std::runtime_error("{{Exception}} Counter dinn't start");
#endif

		mElapsedDur = std::chrono::high_resolution_clock::now() - mStartClock;
		mEndFlag = true;
	}

	double get_elapsed_msec()
	{
#if _DEBUG
		if (!mStartFlag || !mEndFlag)
			throw std::runtime_error("{{Exception}} Counter didn't start or end");
#endif

		return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(mElapsedDur).count();
	}

	double get_elapsed_nanosec()
	{
#if _DEBUG
		if (!mStartFlag || !mEndFlag)
			throw std::runtime_error("{{Exception}} Counter didn't start or end");
#endif

		return mElapsedDur.count();
	}

	double get_elapsed_sec()
	{
#if _DEBUG
		if (!mStartFlag || !mEndFlag)
			throw std::runtime_error("{{Exception}} Counter didn't start or end");
#endif

		return std::chrono::duration_cast<std::chrono::duration<double>>(mElapsedDur).count();
	}
	
private:
	std::chrono::high_resolution_clock::time_point mStartClock;
	std::chrono::duration<double, std::nano> mElapsedDur;
	
#if _DEBUG
	bool mStartFlag = false;
	bool mEndFlag = false;
#endif

};