#include <iostream>

template<class T>
class MemoryPool
{
public:
	const static std::size_t SIZE = 100000;

	MemoryPool()
	{
		for (auto i = 1; i < SIZE; ++i)
		{
			mPool[i - 1].mNext = &mPool[i];
		}

		mNextFree = &mPool[0];
	}

	MemoryPool(const MemoryPool&) = delete;

	MemoryPool(MemoryPool&& other) noexcept
		: mPool{ std::move(other.mPool) }
		, mNextFree{ other.mNextFree }
	{
		other.mNextFree = nullptr;
	}

	~MemoryPool() = default;

	T* allocate()
	{
		if (mNextFree == nullptr)
			throw std::bad_alloc{};

		const auto item = mNextFree;
		mNextFree = item->mNext;

		return reinterpret_cast<T*>(&item->mStorage);
	}

	void deallocate(T* p) noexcept
	{
		const auto item = reinterpret_cast<Item*>(p);

		item->mNext = mNextFree;
		mNextFree = item;
	}

	template<class ...Args>
	T* acquire(Args&& ...args)
	{
		return new (allocate()) T(std::forward<Args>(args)...);
	}

	void release(T* p) noexcept
	{
		if (p == nullptr)
			return;

		p->~T();
		deallocate(p);
	}

	MemoryPool& operator =(const MemoryPool&) = delete;

	MemoryPool& operator =(MemoryPool&& other) noexcept
	{
		if (this == &other)
			return *this;

		mPool = std::move(other.mPool);
		mNextFree = other.mNextFree;

		other.mNextFree = nullptr;

		return *this;
	}

private:
	struct Item
	{
		std::aligned_storage_t<sizeof(T), alignof(T)> mStorage;
		Item* mNext;
	};

	std::unique_ptr<Item[]> mPool = std::make_unique<Item[]>(SIZE);
	Item* mNextFree = nullptr;
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
		mClock = clock();
	}

	int ElapsedMSec()
	{
		return difftime(clock(), mClock);
	}

private:
	clock_t mClock;
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

	int loopCount = 1000000;

	timer.ReStart();
	for (auto i = 0; i < loopCount; ++i)
	{
		auto item = new TestItem();
		delete item;
	}
	std::cout << "NO_USE_POOL Elased: " << timer.ElapsedMSec() << "ms" << std::endl;

	timer.ReStart();
	for (auto i = 0; i < loopCount; ++i)
	{
		auto item = pool.acquire();

		pool.release(item);
	}
	std::cout << "USE POOL Elased: " << timer.ElapsedMSec() << "ms" << std::endl;
}