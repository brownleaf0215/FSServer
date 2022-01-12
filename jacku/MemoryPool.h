#pragma once
#include <Windows.h>
#include <minwinbase.h>
#include <synchapi.h>


class CriticalSection
{
public:
	CriticalSection()
	{
		InitializeCriticalSection(&mSync);
	}
	
	~CriticalSection()
	{
		DeleteCriticalSection(&mSync);
	}

	inline void Enter()
	{
		EnterCriticalSection(&mSync);
	}

	inline void Leave()
	{
		LeaveCriticalSection(&mSync);
	}
private:
	CRITICAL_SECTION mSync;
};

template <class T>
class MultiThreadSync
{
	friend class ThreadSync;
public:
	class ThreadSync
	{
	public:
		ThreadSync()
		{
			T::mSync.Enter();
		}

		~ThreadSync()
		{
			T::mSync.Leave();
		}
	};

private:
	static CriticalSection mSync;
};

template <class T>
CriticalSection MultiThreadSync<T>::mSync;

template <class T, int ALLOC_BLOCK_SIZE = 50>
class MemoryPool
//	: public MultiThreadSync<T>
{
public:
	static void* operator new(std::size_t alloc_length)
	{
		//ThreadSync sync;

		//assert(sizeof(T) == alloc_length);
		//assert(sizeof(T) >= sizeof(UCHAR*));

		if (!m_free_pointer)
			allocBlock();

		UCHAR* return_pointer = m_free_pointer;
		m_free_pointer = *reinterpret_cast<UCHAR**>(return_pointer);

		return return_pointer;
	}

	static void operator delete(void* delete_pointer)
	{
		//ThreadSync sync;
		*reinterpret_cast<UCHAR**>(delete_pointer) = m_free_pointer;
		m_free_pointer = static_cast<UCHAR*>(delete_pointer);
	}
private:
	static void allocBlock()
	{
		m_free_pointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];

		UCHAR** current = reinterpret_cast<UCHAR**>(m_free_pointer);
		UCHAR* next = m_free_pointer;

		for (int i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i)
		{
			next += sizeof(T);
			*current = next;
			current = reinterpret_cast<UCHAR**>(next);
		}

		*current = 0;
	}

private:
	static UCHAR* m_free_pointer;

protected:
	~MemoryPool()
	{
	}
};

template <class T, int ALLOC_BLOCK_SIZE>
UCHAR* MemoryPool<T, ALLOC_BLOCK_SIZE>::m_free_pointer;
