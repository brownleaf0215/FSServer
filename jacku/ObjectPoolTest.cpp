#include "ObjectPoolTest.h"

ObjectPoolTest::ObjectPoolTest() :m_a(0), m_b('0')
{
	std::cout << "Construct default Test" << *this << "\n";
}

ObjectPoolTest::ObjectPoolTest(int a, char b) : m_a(a), m_b(b)
{
	std::cout << "Construct Test" << *this << "\n";
}

ObjectPoolTest::ObjectPoolTest(const ObjectPoolTest& orig) : m_a(orig.m_a), m_b(orig.m_b)
{
	std::cout << "Construct cpy Test" << *this << "\n";
}

ObjectPoolTest::~ObjectPoolTest()
{
	std::cout << "Delete Test" << *this << "\n";
}

std::ostream& operator<<(std::ostream& ostream, const ObjectPoolTest& test)
{
	ostream << "[" << test.m_a << ",'" << test.m_b << "']";
	return ostream;
}

void Initialiser::operator() (ObjectPoolTest* element) 
{
	//std::cout << "Initailising Test" << std::endl;
}

void Releaser::operator() (ObjectPoolTest* element) 
{
	//std::cout << "Releasing Test" << std::endl;
}
