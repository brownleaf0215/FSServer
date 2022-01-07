#pragma once

#include <chrono>
#include <stdexcept>


class MyCounter
{
public:
	MyCounter()
	{
	}
	~MyCounter() = default;

	void start()
	{
		mStartClock = std::chrono::high_resolution_clock::now();
		mStartFlag = true;
	}

	void end()
	{
		if (!mStartFlag)
			throw std::runtime_error("{{Exception}} Counter dinn't start");

		mElapsedDur = std::chrono::high_resolution_clock::now() - mStartClock;
		mEndFlag = true;
	}

	double get_elapsed_msec()
	{
		if (!mStartFlag || !mEndFlag)
			throw std::runtime_error("{{Exception}} Counter didn't start or end");

		return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(mElapsedDur).count();
	}

	double get_elapsed_nanosec()
	{
		if (!mStartFlag || !mEndFlag)
			throw std::runtime_error("{{Exception}} Counter didn't start or end");

		return mElapsedDur.count();
	}

	double get_elapsed_sec()
	{
		if (!mStartFlag || !mEndFlag)
			throw std::runtime_error("{{Exception}} Counter didn't start or end");

		return std::chrono::duration_cast<std::chrono::duration<double>>(mElapsedDur).count();
	}
	
private:
	std::chrono::high_resolution_clock::time_point mStartClock;
	std::chrono::duration<double, std::nano> mElapsedDur;
	
	bool mStartFlag = false;
	bool mEndFlag = false;

};