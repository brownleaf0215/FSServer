#ifndef HEADER_CHUNK_H
#define HEADER_CHUNK_H

#include <vector>

#include <iostream>

#if _DEBUG
#include <stdexcept>
#endif

#include "ConstExpr_Math.h"

namespace raylee
{

#define MAX_SLOT_SIZE			(1024*1024ULL)
#define MIN_CHUCK_SLOT_COUNT	(16ULL)
#define MAX_CHUNK_SLOT_COUNT	(256ULL)
#define MAX_CHUNK_SIZE			(MAX_SLOT_SIZE * MIN_CHUCK_SLOT_COUNT)

	
	class Chunk
	{
	public:
		struct NodeHelper
		{
			void* chunk;
		};


		Chunk(std::size_t obj_size, std::size_t count)
			: m_total_slot_count(static_cast<int32_t>(count))
			, m_current_slot(static_cast<int32_t>(count) - 1)
			, m_data_pointer(count)
			, m_obj_size(obj_size)
		{
			m_total_size = obj_size * count;
			m_used_size = 0;
			m_data = new uint8_t[obj_size * count];

			for (int i = 0; i < count; ++i)
			{
				reinterpret_cast<NodeHelper*>(m_data + i * obj_size)->chunk = this;
				m_data_pointer[i] = m_data + i * obj_size;
			}

			// std::cout << "New Chunk = " << this << std::hex << std::endl;
		}
		~Chunk() = default;

		Chunk(const Chunk&) = delete;
		Chunk(Chunk&& src) = delete;

		Chunk& operator=(Chunk&& rhs) = delete;
		//{
		//	this->~Chunk();
		//	new(this) Chunk(std::move(rhs));
		//	return *this;
		//}

		double get_empty_ratio() 
		{
#if _DEBUG
			if (0 == m_total_slot_count)
				throw new std::runtime_error("empty chunk");
#endif
			return (m_total_slot_count - m_current_slot + 1) / (double)m_total_slot_count;
		}

		void* alloc()
		{
#if _DEBUG
			if(empty())
				throw new std::runtime_error("request to empty chunk");
#endif
			
			uint8_t* p = m_data_pointer[m_current_slot];
			--m_current_slot;

			// std::cout << "Alloc = " << (void*)p << std::hex << std::endl;

			return p + sizeof(void*);
		}

		void dealloc(uint8_t* p)
		{
#if _DEBUG
			if (full())
				throw new std::runtime_error("request to full chunk");
#endif
			++m_current_slot;
			m_data_pointer[m_current_slot] = p;
		}

		bool empty() 
		{
			return (m_current_slot < 0);
		}

		bool full()
		{
			return (m_current_slot == m_total_slot_count-1);
		}

		bool check_enouph_threshhold()
		{
			return get_empty_ratio() > empty_threshold;
		}

		inline void set_empty()
		{
			m_empty_check = true;
		}
		inline bool get_empty_flag()
		{
			return m_empty_check;
		}
		inline void set_enouph()
		{
			m_empty_check = false;
		}


	private:
		int32_t m_total_slot_count;		// 전체 슬롯 개수
		int32_t m_current_slot;			// 메모리 슬롯의 위치
		std::size_t m_total_size;		// 전체 메모리 크기
		std::size_t m_used_size;		// 사용된 메모리 크기
		const std::size_t m_obj_size;
		uint8_t* m_data = nullptr;
		std::vector<uint8_t*> m_data_pointer;
		bool m_empty_check = false;

		const double empty_threshold = 0.15f;
		// static const std::size_t ObjectSize = CE_MEMSIZE(sizeof(ObjectT));
	};
}

#endif
