// Pools.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "ObjectPool.h"
#include "ObjectPoolTest.h"
#include "TimeLog.h"
#include "ThreadPool.h"
#include <iomanip>
#include "MemoryPool.h"

void useMakeUnique()
{
    TimeLog time_log;
    std::cout << "[Makeunique]" << std::endl;
    std::unique_ptr<ObjectPoolTest> pt1, pt2, pt3, pt4, pt5, pt6, pt7;

    time_log.check();

    pt1 = std::make_unique<ObjectPoolTest>();
    std::cout << "Acquired pt:" << *pt1 << std::endl;
    time_log.check();
    
    pt1.reset();
    std::cout << "Reset pt:" << std::endl;
    time_log.check();

    pt2 = std::make_unique<ObjectPoolTest>();
    std::cout << "Acquired pt:" << *pt2 << std::endl;
    time_log.check();

    pt2.reset();
    std::cout << "Reset pt:" << std::endl;
    time_log.check();

    pt3 = std::make_unique<ObjectPoolTest>();
    std::cout << "Acquired pt:" << *pt3 << std::endl;
    time_log.check();

    pt3.reset();
    std::cout << "Reset pt:" << std::endl;
    time_log.check();

    pt4 = std::make_unique<ObjectPoolTest>();
    std::cout << "Acquired pt:" << *pt4 << std::endl;
    time_log.check();

    pt4.reset();
    std::cout << "Reset pt:" << std::endl;
    time_log.check();

    pt5 = std::make_unique<ObjectPoolTest>();
    std::cout << "Acquired pt:" << *pt5 << std::endl;
    time_log.check();

    pt5.reset();
    std::cout << "Reset pt:" << std::endl;
    time_log.check();

    pt6 = std::make_unique<ObjectPoolTest>();
    std::cout << "Acquired pt:" << *pt6 << std::endl;
    time_log.check();

    pt6.reset();
    std::cout << "Reset pt:" << std::endl;
    time_log.check();

}

void useObjectPool()
{
    TimeLog time_log;
    std::cout << "[Objectpool]" << std::endl;
    using TestPool = ObjectPool<ObjectPoolTest, 5, Initialiser, Releaser>;
    TestPool pool;
    TestPool::pointer pt1, pt2, pt3, pt4, pt5, pt6, pt7;

    //std::cout << pool;
    
    time_log.check();
    
    pt1 = pool.acquire();
    std::cout << "Acquired pt:" << *pt1 << std::endl;
    time_log.check();

    pt1.reset();
    std::cout << "Reset pt:" << std::endl;
    time_log.check();

    //std::cout << pool;

    pt2 = pool.acquire();
    std::cout << "Acquired pt:" << *pt2 << std::endl;
    time_log.check();

    pt2.reset();
    std::cout << "Reset pt:" << std::endl;
    time_log.check();
    //std::cout << pool;

    pt3 = pool.acquire();
    std::cout << "Acquired pt:" << *pt3 << std::endl;
    time_log.check();

    pt3.reset();
    std::cout << "Reset pt:" << std::endl;
    time_log.check();
    //std::cout << pool;

    pt4 = pool.acquire();
    std::cout << "Acquired pt:" << *pt4 << std::endl;
    time_log.check();

    pt4.reset();
    std::cout << "Reset pt:" << std::endl;
    time_log.check();
    //std::cout << pool;

    pt5 = pool.acquire();
    std::cout << "Acquired pt:" << *pt5 << std::endl;
    time_log.check();

    pt5.reset();
    std::cout << "Reset pt:" << std::endl;
    time_log.check();
    //std::cout << pool;

    pt6 = pool.acquire();
    std::cout << "Acquired pt:" << *pt6 << std::endl;
    time_log.check();

    pt6.reset();
    std::cout << "Reset pt:" << std::endl;
    time_log.check();
    //std::cout << pool;

}

int threadPoolWork(int t, int id)
{
    std::cout << id << " start" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(t));
    std::cout << id << " end after " << t << std::endl;
    
    return t + id;
}

void threadWork(int t1)
{
    std::cout << "Thread: " << t1 << std::endl;
    
    auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    std::cout << "milliseconds since epoch: " << millisec_since_epoch << std::endl;
}

void currentTime()
{
    auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    std::cout << "milliseconds since epoch: " << millisec_since_epoch << std::endl;
}

void useThreadPool()
{
    currentTime();
    TimeLog time_log;

    ThreadPool pool(3);
    currentTime();
    time_log.check();

    for (int i = 0; i < 3; i++)
    {
        pool.EnqueueJob(threadWork, i);
    }

    time_log.check();
}

void useThread()
{
    currentTime();
    TimeLog time_log;

    std::vector<std::thread> threads(3);
    int i = 0;
    for (auto& t : threads) 
    {
        t = std::thread{ threadWork, i };
        i++;
    }

    for (auto& t : threads) 
    {
        t.join();
    }
    time_log.check();
}

class DataMP
    :public MemoryPool<DataMP>
{
private:
    BYTE a[1024];
    BYTE b[1024];
};

class Data
{
private:
    BYTE a[1024];
    BYTE b[1024];
};

void useMemoryPool()
{
    TimeLog time_log;
    for (int i = 0; i < 1000000; i++)
    {
        DataMP* p_data = new DataMP();

        delete p_data;
    }
    time_log.check();
}

void useMemory()
{
    TimeLog time_log;
    for (int i = 0; i < 1000000; i++)
    {
        Data* p_data = new Data();

        delete p_data;
    }
    time_log.check();
}


int main()
{
    // 1. ObjectPool 사용하여 시간 체크
    // useObjectPool();

    // 2. ObjectPool 안 사용하여 시간 체크
    // useMakeUnique();

    // 3. ThreadPool 사용
    //useThreadPool();

    // 4. Thread 사용
    // useThread();
    
    // 5. MemoryPool 사용
    //useMemoryPool();

    // 6. Memory 사용
    useMemory();

    return 0;
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
