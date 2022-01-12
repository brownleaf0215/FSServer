#pragma once

#include <iostream>
#include <memory>
#include <functional>

/**
* Generic class for object pools.
*/
template <class T, int SZ, class Initialiser, class Releaser>
class ObjectPool
{
public:
	using pointer = std::unique_ptr<T, std::function<void(T*)> >;
	using PoolType = ObjectPool<T, SZ, Initialiser, Releaser>;

	ObjectPool() {}
	ObjectPool(const PoolType& orig) = delete;
	~ObjectPool() {}

	/**
	*	Acquires an object not being currently used
	* @return pointer to the acquired object
	* @throw std::out_of_range if all the objects inside the pool are being used
	*/
	pointer acquire() {
		unsigned int index = 0;		// look for first free object
		while (m_occupied_registry[index])
			++index;
		if (index >= SZ)
			throw std::out_of_range("Pool exceeded its size");
		m_occupied_registry[index] = true;	// mark it as currently in use
		m_initialiser(&m_objects[index]);	// initialise it

		// return an unique_ptr that calls release when reset
		return pointer(&m_objects[index], [this](T* element)->void {release(element); });
	}

	friend std::ostream& operator<<(std::ostream& ostream, const PoolType& pool)
	{
		for (unsigned int index = 0; index < SZ; index++)
		{
			std::cout << (pool.m_occupied_registry[index] ? "[occupied] " : "[free]\n");
			if (pool.m_occupied_registry[index])
			{
				std::cout << pool.m_objects[index] << std::endl;
			}
		}
		return ostream;
	}

private:
	void release(T* element)
	{
		unsigned int index = 0;
		while (&m_objects[index] != element)
			++index;
		m_occupied_registry[index] = false;
		m_releaser(element);	// call release functor
	}

	Initialiser m_initialiser;
	Releaser m_releaser;
	bool m_occupied_registry[SZ]{ 0 };
	T m_objects[SZ];
};
