#pragma once

enum
{
	SLIST_ALIGNMENT = 16
};

DECLSPEC_ALIGN(SLIST_ALIGNMENT)
/// @brief 메모리 풀에서 사용하기위한 헤더 정보
struct MemoryHeader : public SLIST_ENTRY
{
	// [MemoryHeader][Data]
	MemoryHeader(int32 size) : alloc_size(size) { }

	/// @brief 
	/// @param header 헤더를 할당할 포인터
	/// @param size 
	/// @return 데이터의 시작점 반환 
	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size); // placement new
		return reinterpret_cast<void*>(++header);
	}

	/// @brief 
	/// @param ptr 데이터의 포인터
	/// @return 메모리 헤더의 시작점 반환
	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 alloc_size;
};

DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void Push(MemoryHeader* ptr);
	MemoryHeader* Pop();

private:
	SLIST_HEADER m_header;
	int32 m_alloc_size = 0;
	atomic<int32> m_alloc_count = 0;
};

