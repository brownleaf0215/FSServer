#include "pch.h"
#include "Memory.h"
#include "MemoryPool.h"

Memory::Memory()
{
	int32 size = 0;
	int32 table_index = 0;

	for (size = 32; size <= 1024; size += 32)
	{
		MemoryPool* pool = new MemoryPool(size);
		m_pools.push_back(pool);

		while (table_index <= size)
		{
			m_pool_table[table_index] = pool;
			table_index++;
		}
	}

	for (size = 1024; size <= 2048; size += 128)
	{
		MemoryPool* pool = new MemoryPool(size);
		m_pools.push_back(pool);

		while (table_index <= size)
		{
			m_pool_table[table_index] = pool;
			table_index++;
		}
	}

	for (size = 2048; size <= 4096; size += 256)
	{
		MemoryPool* pool = new MemoryPool(size);
		m_pools.push_back(pool);

		while (table_index <= size)
		{
			m_pool_table[table_index] = pool;
			table_index++;
		}
	}
}

Memory::~Memory()
{
	for (MemoryPool* pool : m_pools)
		delete pool;

	m_pools.clear();
}

void* Memory::Allocate(int32 size)
{
	MemoryHeader* header = nullptr;

	const int32 alloc_size = size + sizeof(MemoryHeader);

	if (alloc_size > MAX_ALLOC_SIZE)
	{
		// 메모리 풀링 최대 크기를 벗어나면 일반 할당
		header = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(alloc_size, SLIST_ALIGNMENT));
	}
	else
	{
		// 메모리 풀에서 꺼내온다
		header = m_pool_table[alloc_size]->Pop();
	}

	return MemoryHeader::AttachHeader(header, alloc_size);
}

void Memory::Release(void* ptr)
{
	MemoryHeader* header = MemoryHeader::DetachHeader(ptr);

	const int32 alloc_size = header->alloc_size;
	// ASSERT_CRASH(allocSize > 0);

	if (alloc_size > MAX_ALLOC_SIZE)
	{
		// 메모리 풀링 최대 크기를 벗어나면 일반 해제
		::_aligned_free(header);
	}
	else
	{
		// 메모리 풀에 반환
		m_pools[alloc_size]->Push(header);
	}
}
