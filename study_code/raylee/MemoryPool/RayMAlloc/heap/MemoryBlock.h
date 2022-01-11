#ifndef HEADER_MEMORY_BLOCK_H
#define HEADER_MEMORY_BLOCK_H

#include <list>
#include <algorithm>
#include <unordered_map>
#include <tuple>
#include <cassert>
#include "Chunk.h"

namespace raylee
{
	class MemoryBlock
	{
	public:
		MemoryBlock(std::size_t obj_size)
			: m_obj_size(obj_size)
		{
			assert(m_obj_size <= MAX_SLOT_SIZE);

			m_slot_count = MAX_CHUNK_SIZE / m_obj_size;
			m_slot_count = std::min(MAX_CHUNK_SLOT_COUNT, m_slot_count);
			m_slot_count = std::max(MIN_CHUCK_SLOT_COUNT, m_slot_count);

			// 처음에 하나 만들어 놓는다.
			make_chunk();
		}

		void* alloc()
		{
			if (empty())
				make_chunk();

			Chunk* chunk = m_free_chunks.back();
			void* p = chunk->alloc();
			if (chunk->empty())
			{
				m_empty_chunks.push_back(chunk);
				chunk->set_empty();
				m_free_chunks.pop_back();
			}
			return p;
		}

		void dealloc(void* obj)
		{
			if (nullptr == obj)
				return;

			uint8_t* p = reinterpret_cast<uint8_t*>(obj) - sizeof(void*);

#if _DEBUG
			//std::cout << "Obj = " << obj << std::hex << std::endl;
			//std::cout << "p   = " << (void*)p << std::hex << std::endl;
#endif

			Chunk* chunk = reinterpret_cast<Chunk*>(reinterpret_cast<Chunk::NodeHelper*>(p)->chunk);
			chunk->dealloc(p);

			if (chunk->get_empty_flag() && chunk->check_enouph_threshhold())
			{
				const auto end = std::end(m_empty_chunks);
				const auto iter = std::find(std::begin(m_empty_chunks), end, chunk);
				if (iter != end)
				{
					m_empty_chunks.erase(iter);
					m_free_chunks.push_front(chunk);
				}
			}

		}

	private:
		void make_chunk()
		{
			Chunk * new_chunk =
				new Chunk(m_obj_size, m_slot_count);

			m_free_chunks.push_back(new_chunk);
		}
		
		inline bool empty()
		{
			return m_free_chunks.empty();
		}

	public: 
		std::size_t GetObjectSize() {
			return m_obj_size;
		}

		std::size_t GetSlotCount() {
			return m_slot_count;
		}

	private:
		std::size_t m_obj_size;
		std::size_t m_slot_count;

		std::list<Chunk*> m_free_chunks;
		std::list<Chunk*> m_empty_chunks;
	};
	
}



#endif