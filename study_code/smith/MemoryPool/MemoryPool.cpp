#include <iostream>

template<class T>
class MemoryPool
{
public:
	const static std::size_t POOL_SIZE = 100000;

	MemoryPool()
	{
		for (auto i = 1; i < POOL_SIZE; ++i)
		{
			m_pool[i - 1].m_next = &m_pool[i];
		}

		m_next_free = &m_pool[0];
	}

	MemoryPool(const MemoryPool&) = delete;

	MemoryPool(MemoryPool&& other) noexcept
		: m_pool{ std::move(other.m_pool) }
		, m_next_free{ other.m_next_free }
	{
		other.m_next_free = nullptr;
	}

	~MemoryPool() = default;

	T* Allocate()
	{
		if (m_next_free == nullptr)
			throw std::bad_alloc{};

		const auto item = m_next_free;
		m_next_free = item->m_next;

		return reinterpret_cast<T*>(&item->m_storage);
	}

	void Deallocate(T* p) noexcept
	{
		const auto item = reinterpret_cast<Item*>(p);

		item->m_next = m_next_free;
		m_next_free = item;
	}

	template<class ...Args>
	T* Acquire(Args&& ...args)
	{
		return new (Allocate()) T(std::forward<Args>(args)...);
	}

	void Release(T* p) noexcept
	{
		if (p == nullptr)
			return;

		p->~T();
		Deallocate(p);
	}

	MemoryPool& operator =(const MemoryPool&) = delete;

	MemoryPool& operator =(MemoryPool&& other) noexcept
	{
		if (this == &other)
			return *this;

		m_pool = std::move(other.m_pool);
		m_next_free = other.m_next_free;

		other.m_next_free = nullptr;

		return *this;
	}

private:
	struct Item
	{
		std::aligned_storage_t<sizeof(T), alignof(T)> m_storage;
		Item* m_next;
	};

	std::unique_ptr<Item[]> m_pool = std::make_unique<Item[]>(POOL_SIZE);
	Item* m_next_free = nullptr;
};


class Timer
{
public:
	Timer() = default;

	~Timer()
	{
	}

	void ReStart()
	{
		m_clock = clock();
	}

	int ElapsedMSec()
	{
		return difftime(clock(), m_clock);
	}

private:
	clock_t m_clock;
};

struct TestItem
{
public:
	int value = 0;
};

void main()
{
	Timer timer;
	MemoryPool<TestItem> pool;

	int loop_count = 1000000;

	timer.ReStart();
	for (auto i = 0; i < loop_count; ++i)
	{
		auto item = new TestItem();
		delete item;
	}
	std::cout << "NO_USE_POOL Elased: " << timer.ElapsedMSec() << "ms" << std::endl;

	timer.ReStart();
	for (auto i = 0; i < loop_count; ++i)
	{
		auto item = pool.Acquire();

		pool.Release(item);
	}
	std::cout << "USE POOL Elased: " << timer.ElapsedMSec() << "ms" << std::endl;
}