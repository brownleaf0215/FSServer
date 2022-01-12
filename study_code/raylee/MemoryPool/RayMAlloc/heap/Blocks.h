#ifndef HEADER_BLOCKS_H
#define HEADER_BLOCKS_H

#include <vector>

#include <iostream>

#if _DEBUG
#include <stdexcept>
#endif

#include "ConstExpr_Math.h"

namespace raylee
{

#define MAX_SLOT_SIZE			(1024*1024ULL)
#define MIN_CHUCK_SLOT_COUNT	(10ULL)
#define MAX_CHUNK_SLOT_COUNT	(256ULL)
#define MAX_CHUNK_SIZE			(MAX_SLOT_SIZE * MIN_CHUCK_SLOT_COUNT)


	template<std::size_t ObjSize>
	class Slot
	{
	public:
		Slot()
		{
		}
		~Slot()
		{
		}

		Slot(const Slot&) = delete;
		Slot(Slot&& src) = delete;

		Slot& operator=(const Slot& rhs) = delete;
		Slot& operator=(Slot&& rhs) = delete;

		void set(void* chunk)
		{
			m_node.chunk = chunk;
		}

		void* get()
		{
			return (void*)(m_node.data + sizeof(void*));
		}

		void* get_parent()
		{
			return m_node.chunk;
		}

	private:
		union Node
		{
			uint8_t data[sizeof(void*) + ObjSize];
			void* chunk = nullptr;
		} m_node;
	};


	
	template<std::size_t ObjSize>
	class Chunk
	{
	public:
		using SlotT = Slot<ObjSize>;
		// using Pointer = Chunk<ObjectT>*;

		Chunk(std::size_t count)
			: m_total_slot_count(count)
			, m_current_slot(count - 1)
			, m_slots(count)
			, m_slot_pointer(count)
		{
			for (int i = 0; i < count; ++i)
			{
				m_slots[i].set(this);
				m_slot_pointer[i] = &m_slots[i];
			}

			std::cout << "New Chunk = " << this << std::hex << std::endl;
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

		double GetEmptyRatio() 
		{
#if _DEBUG
			if (0 == m_total_slot_count)
				throw new std::runtime_error("empty chunk");
#endif
			return (m_total_slot_count - m_current_slot + 1) / (double)m_total_slot_count;
		}

		SlotT* alloc()
		{
#if _DEBUG
			if(empty())
				throw new std::runtime_error("request to empty chunk");
#endif
			
			SlotT* p = m_slot_pointer[m_current_slot];
			--m_current_slot;

			std::cout << "Alloc = " << (void*)p << std::hex << std::endl;

			return p;
		}

		void dealloc(SlotT* slot)
		{
#if _DEBUG
			if (full())
				throw new std::runtime_error("request to full chunk");
#endif
			++m_current_slot;
			m_slot_pointer[m_current_slot] = slot;
		}

		bool empty() 
		{
			return (m_current_slot < 0);
		}

		bool full()
		{
			return (m_current_slot == m_total_slot_count-1);
		}

	private:
		std::size_t m_total_slot_count;		// 전체 슬롯 개수
		std::size_t m_current_slot;			// 메모리 슬롯의 위치
		std::size_t m_total_size;	// 전체 메모리 크기
		std::size_t m_used_size;		// 사용된 매무리 크기
		std::vector<SlotT> m_slots;
		std::vector<SlotT*> m_slot_pointer;

		const double empty_threshhole = 0.80f;

		// static const std::size_t ObjectSize = CE_MEMSIZE(sizeof(ObjectT));
	};
}

#endif
