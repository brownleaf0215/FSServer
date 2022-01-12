#pragma once
#include <future>
#include <queue>

class ThreadPool
{
public:
	ThreadPool(size_t num_threads);
	~ThreadPool();

	template <class F, class... Args>
	std::future<typename std::result_of<F(Args...)>::type> EnqueueJob(F&& f, Args&&... args);

private:
	void WorkerThread();

	size_t m_num_threads;
	std::vector<std::thread> m_worker_threads;
	std::queue<std::function<void()>> m_jobs;
	std::condition_variable m_cv_job_q;
	std::mutex m_m_job_q;
	bool m_stop_all;
};

template <class F, class... Args>
std::future<typename std::result_of<F(Args...)>::type> ThreadPool::EnqueueJob(F&& f, Args&&... args)
{
	if (m_stop_all)
	{
		throw std::runtime_error("ThreadPool stop!!");
	}

	using return_type = typename std::result_of<F(Args...)>::type;
	auto job = std::make_shared<std::packaged_task<return_type()>>(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...));
	std::future<return_type> job_result_future = job->get_future();
	{
		std::lock_guard<std::mutex> lock(m_m_job_q);
		m_jobs.push([job]() { (*job)();  });
	}
	m_cv_job_q.notify_one();

	return job_result_future;
}
