#ifndef HEADER_MEMORY_POOL_H
#define HEADER_MEMORY_POOL_H

#include <list>
#include <algorithm>
#include <unordered_map>
#include <tuple>
#include <cassert>
#include "MemoryBlock.h"

namespace raylee
{
	class MemoryPool
	{
	public:
		MemoryPool() = default;
		~MemoryPool() = default;

		template<typename T>
		constexpr decltype(auto) GetPool()
		{
			const std::size_t size = POOL_MEMSIZE(sizeof(T));
			static_assert(size <= MAX_CHUNK_SIZE);

			const int32_t index = ce_find_in_pair(m_pool, size, std::size(m_pool));

			return m_pool[index].second;
		}

		template<typename T>
		T* alloc()
		{
			raylee::MemoryBlock* block = nullptr;
			block = GetPool<T>();
			if (nullptr != block)
				return reinterpret_cast<T*>(block->alloc());
			
			return new T();
		}

		template<typename T>
		void dealloc(T* p)
		{
			raylee::MemoryBlock* block = nullptr;
			block = GetPool<T>();
			if (nullptr != block)
				block->dealloc(p);
			else
				delete p;
		}

	private:

#define RAY_HELPER(x) std::make_pair(x, new MemoryBlock(x))

		const std::pair<std::size_t, MemoryBlock *> m_pool[15] = {
			RAY_HELPER(64),
			RAY_HELPER(128), 
			RAY_HELPER(256), 
			RAY_HELPER(512), 
			RAY_HELPER(1024), 
			RAY_HELPER(1024 * 2),
			RAY_HELPER(1024 * 4),
			RAY_HELPER(1024 * 8),
			RAY_HELPER(1024 * 16),
			RAY_HELPER(1024 * 32),
			RAY_HELPER(1024 * 64),
			RAY_HELPER(1024 * 128),
			RAY_HELPER(1024 * 256),
			RAY_HELPER(1024 * 512),
			RAY_HELPER(1024 * 1024) 
		};

#undef RAY_HELPER

	};

	


	
}



#endif