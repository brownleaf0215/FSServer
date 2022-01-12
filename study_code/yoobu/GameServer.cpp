#include "pch.h"
#include "CorePch.h"
#include "Memory.h"

Memory MEMORY;

class Monster
{
public:
	int32 _hp;
};

void CreateAndDeleteMonster()
{
	for (int32 i = 0; i < 1'000'000; ++i)
	{
		Monster* monster = (Monster*)MEMORY.Allocate(sizeof(Monster));
		monster->_hp = i;
		cout << monster->_hp << endl;

		this_thread::sleep_for(10ms);

		MEMORY.Release(monster);
	}
}



int main()
{
	list<thread> thread_list;

	for (int32 i = 0; i < 5; ++i)
	{
		thread t(CreateAndDeleteMonster);

		thread_list.push_back(std::move(t));
	}

	for (auto& t : thread_list)
	{
		if (t.joinable())
			t.join();
	}
}

