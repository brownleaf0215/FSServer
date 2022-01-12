#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t num_threads)
	: m_num_threads(num_threads), m_stop_all(false)
{
	m_worker_threads.reserve(m_num_threads);
	for (size_t i = 0; i < m_num_threads; ++i)
	{
		m_worker_threads.emplace_back([this]() { this->WorkerThread(); });
	}
}

void ThreadPool::WorkerThread()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(m_m_job_q);
		m_cv_job_q.wait(lock, [this]() { return !this->m_jobs.empty() || m_stop_all; });
		if (m_stop_all && this->m_jobs.empty())
		{
			return;
		}

		std::function<void()> job = std::move(m_jobs.front());
		m_jobs.pop();
		lock.unlock();

		job();
	}
}

ThreadPool::~ThreadPool()
{
	m_stop_all = true;
	m_cv_job_q.notify_all();

	for (auto& t : m_worker_threads)
	{
		t.join();
	}
}

