// RayMAlloc.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

// Ray's Multi-thread Allocator ( Memory Pool )
// It gets idea from HOARD
// Thread-Heap ( includes chunk list ) and Global Heap. ( Same structure )
/*
malloc(sz)
1. If sz > S = 2, allocate the superblock from the OS
and return it.
2. i ˆ hash(the current thread).
3. Lock heap i.
4. Scan heap i’s list of superblocks from most full to least
(for the size class corresponding to sz).
5. If there is no superblock with free space,
6. Check heap 0 (the global heap) for a superblock.
7. If there is none,
    8. Allocate S bytes as superblock s
    and set the owner to heap i.
9. Else,
    10. Transfer the superblock s to heap i.
    11. u0 ˆ u0 ¡ s : u
    12. ui ˆ ui + s : u
    13. a0 ˆ a0 ¡ S
    14. ai ˆ ai + S
    15. ui ˆ ui + sz.
    16. s : u ˆ s : u + sz.
    17. Unlock heap i.
    18. Return a block from the superblock

free(ptr)
1. If the block is “large”,
2. Free the superblock to the operating system and return.
3. Find the superblock s this block comes from and lock it.
4. Lock heap i, the superblock’s owner.
5. Deallocate the block from the superblock.
6. ui ˆ ui ¡ block size.
7. s:u ˆ s : u ¡ block size.
8. If i = 0, unlock heap i and the superblock
and return.
9. If ui < ai ¡ K ⁄ Sand ui < (1 ¡ f) ⁄ ai,
    10. Transfer a mostly - empty superblock s1
    to heap 0 (the global heap).
    11. u0 ˆ u0 + s1:u, ui ˆ ui ¡ s1 : u
    12. a0 ˆ a0 + S, ai ˆ ai ¡ S
    13. Unlock heap i and the superblock
*/

#include "../../utility/TickCounter.h"

#include "heap/MemoryPool.h"

namespace PoolTest
{
    class Small
    {
    public:
    private:
        uint8_t m_buf[64];
    };

    class Mid
    {
    public:
    private:
        uint8_t m_buf[80];
    };

    class Big
    {
    public:
    private:
        uint8_t m_buf[1024*8];
        uint8_t m_buf2[1024 * 12];
    };


    template<typename T>
    void TestPool(int32_t count, raylee::MemoryPool * pool)
    {

        std::vector<T*> vec(count);

        MyCounter counter;

        counter.start();

        int32_t i = 0;
        while (i < count)
        {
            int32_t alloc = (std::rand() % 800) + 200 ;

            for (int32_t j = 0; j < alloc; ++j)
            {
                vec[j] = pool->alloc<T>();
            }

            for (int32_t j = 0; j < alloc; ++j)
            {
                pool->dealloc(vec[j]);
            }

            i += alloc;
        }

        counter.end();

        std::cout << "Size : " << sizeof(T) << ", Count " << i << " : Elapsed " << counter.get_elapsed_msec() << std::dec << " msec.\n"; 
    }
}


int main()
{
    raylee::MemoryPool* pool = new raylee::MemoryPool();

    PoolTest::TestPool< PoolTest::Small >(100000, pool);
    PoolTest::TestPool< PoolTest::Mid >(100000, pool);
    PoolTest::TestPool< PoolTest::Big >(100000, pool);

    std::cout << "HSize = " << std::hardware_constructive_interference_size << " Hello World!\n";
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
