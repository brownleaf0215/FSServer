#pragma once

#include <Pch.h>
#include <mutex>

using ObjectIndex = int;
using ThreadId = DWORD;
using MemorySize = int;

struct ObjectHeader
{
	ThreadId m_threadid = 0;
	MemorySize m_size = 0;
};

constexpr std::size_t ce_sqrt(std::size_t x, std::size_t y = 0)
{
	if (x == 0)
		return 0;

	if (x == 1)
		return y;
	++y;

	return ce_sqrt(x >> 1, y);
}

constexpr std::size_t ce_2_square(std::size_t x)
{
	if (x > 0)
		return 2 * ce_2_square(x - 1);
	return 1;
}

// x = sizeof(T)일 때, 64, 128, 256, 512, ... 구하는 메크로.
#define CE_MEMSIZE(x) (ce_2_square( ( (x+7)/64 == 0 ? 0 : ce_sqrt((x+7)/64) + 1) + 6) )

template<class T, size_t BLOCK_SIZE = 1024>
class ObjectBuffer 
{
public:
	static const std::size_t Size = CE_MEMSIZE(sizeof(T) + sizeof(ObjectHeader));

	ObjectBuffer() 
	{
	}

	T* Pop()
	{
		if (m_buffer.empty() == true)
			allocate();

		m_use_count++;
		BYTE* buffer = nullptr;
		m_buffer.try_pop(buffer);
		
		return (T*)buffer;
	}

	void Push(T* block)
	{
		m_use_count--;
		m_buffer.push((BYTE*)block);
	}

private:
	void allocate()
	{
		BYTE* page = new BYTE[Size * BLOCK_SIZE];
		ZeroMemory(page, Size * BLOCK_SIZE);
		BYTE* buffer = page;

		for (int index = 0; index < BLOCK_SIZE; ++index)
		{
			ObjectHeader* header = (ObjectHeader*)buffer;
			header->m_threadid = (int)GetCurrentThreadId();
			header->m_size = (int)Size;
			buffer += sizeof(ObjectHeader);

			m_buffer.push((BYTE*)buffer);
			buffer += sizeof(sizeof(T));
		}
	}

private:
	concurrency::concurrent_queue<BYTE*> m_buffer;
	int m_use_count = 0;
};

template <class T>
class ObjectPool
{
public:
	ObjectPool() {}
	virtual ~ObjectPool() {}

	T* Alloc();
	void DeAlloc(T *free);

private:
	concurrency::concurrent_unordered_map<ThreadId, ObjectBuffer<T>*> m_buffer;
};

template<class T>
class SingleObjectPool
{
public:
	SingleObjectPool() {}
	virtual ~SingleObjectPool() {}

	T* Alloc();
	void DeAlloc(T* free);
private:
	ObjectBuffer<T> m_buffer;
	std::mutex m_mutex;
};

#include "ObjectPool.hpp"