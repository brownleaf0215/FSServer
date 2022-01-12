#include "TimeLog.h"


TimeLog::TimeLog()
{
	init();
}

void TimeLog::init()
{
	std::cout << "Initalize time." << std::endl;
	m_record_time = high_resolution_clock::now();
}

void TimeLog::check()
{
	time current_time = high_resolution_clock::now();
	auto diff = current_time - m_record_time;

	std::cout << "diff time: " << duration<double, std::milli>(diff).count() << "ms" << std::endl;

	m_record_time = current_time;
}
