#pragma once
#include <chrono>
#include <iostream>
using namespace std::chrono;

class TimeLog
{
public:
	using time = high_resolution_clock::time_point;

	TimeLog();

	void init();

	void check();

private:
	time m_record_time;
};

