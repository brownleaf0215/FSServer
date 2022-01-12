#pragma once
#include <iostream>
class ObjectPoolTest
{
public:
	ObjectPoolTest();
	ObjectPoolTest(int a, char b);
	ObjectPoolTest(const ObjectPoolTest& orig);
	~ObjectPoolTest();
	friend std::ostream& operator<<(std::ostream& ostream, const ObjectPoolTest& test);
private:
	int m_a;
	char m_b;
};

struct Initialiser {
	void operator() (ObjectPoolTest* element);
};

struct Releaser {
	void operator() (ObjectPoolTest* element);
};