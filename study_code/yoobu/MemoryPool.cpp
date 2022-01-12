#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(int32 alloc_size) : m_alloc_size(alloc_size)
{
	::InitializeSListHead(&m_header);
}

MemoryPool::~MemoryPool()
{
	while (MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&m_header)))
	{
		::_aligned_free(memory);
	}
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	// 반환전 사용되지않음 처리
	ptr->alloc_size = 0;

	::InterlockedPushEntrySList(&m_header, static_cast<PSLIST_ENTRY>(ptr));
	m_alloc_count.fetch_sub(1);
}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&m_header));

	// 없으면 새로 추가
	if (memory == nullptr)
	{
		memory = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(m_alloc_size, SLIST_ALIGNMENT));
	}
	else
	{
		// ASSERT_CRASH(header->allocSize == 0);
	}

	m_alloc_count.fetch_add(1);

	return memory;
}
