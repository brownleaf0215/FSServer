// MemoryPoolLib.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <string_view>
#include "TickCounter.h"

namespace TestObj
{
    class Small
    {
    public:

    private:
        int mA;
        int mB;
        int mC;
    };

    class Medium : public Small
    {
    public:

    private:
        char mBuf[256];
        int mA;
    };

    class Big : public Medium
    {
    private:
        char mAdded1[256];
        char mAdded2[256];
        char mAdded3[256];
    };
}

// Lida 테스트
#define PD_LIDA

#ifdef PD_LIDA

#include "Lida/MemoryPool.hpp"

template<class ObjT>
void Test(std::string_view name)
{
    using Pool = lida::MemoryPool<ObjT>;

    const int count = 100000;
    MyCounter tc;
    tc.start();

    {
        std::vector<ObjT*> aVec(count);
        
        for (int i = 0; i < count; ++i)
            aVec.emplace_back(Pool::allocate(1));

        for (int i = 0; i < count; ++i)
            Pool::deallocate(aVec[i], 1);
    }

    tc.end();

    std::cout << "[" << name << "] Elapsed Time = " << tc.get_elapsed_msec() << " milli seconds\n";
}

int main()
{
    using namespace lida;
    using namespace std;

    Test<TestObj::Small>("Small");
    Test<TestObj::Medium>("Medium");
    Test<TestObj::Big>("Big");

    cout << "End Test\n";
}

#endif


// Lida 테스트
//#define PD_LIDA

//#ifdef PD_LIDA

//#include "Lida/MemoryPool.hpp"

//int main()
//{
//    std::cout << "Hello World!\n";
//}

//#endif




// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
