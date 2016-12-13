#include "ObjectPool.h"
namespace nep
{
	template<typename T, unsigned int SIZE>
	ObjectPool<T, SIZE>::ObjectPool() : m_firstFreeObject(0)
	{
		for (unsigned int i = 0; i < SIZE; i++)
		{
			m_objects[i] = new T();
			m_isUsed[i] = false;
		}
	}

	template<typename T, unsigned int SIZE>
	ObjectPool<T, SIZE>::~ObjectPool()
	{
		for (unsigned int i = 0; i < SIZE; i++)
			delete m_objects[i];
	}

	template<typename T, unsigned int SIZE>
	T* ObjectPool<T, SIZE>::GetObject()
	{
		if (m_firstFreeObject == SIZE)
			return nullptr;

		T* freeItem = m_objects[m_firstFreeObject];
		m_isUsed[m_firstFreeObject] = true;
		m_firstFreeObject++;

		while (m_firstFreeObject < SIZE && m_isUsed[m_firstFreeObject])
			m_firstFreeObject++;

		return freeItem;
	}

	template<typename T, unsigned int SIZE>
	unsigned int ObjectPool<T, SIZE>::ReserveObject()
	{
		unsigned int index = -1;
		if (m_firstFreeObject == SIZE)
			return index;

		index = m_firstFreeObject;
		m_isUsed[m_firstFreeObject] = true;
		m_firstFreeObject++;

		while (m_firstFreeObject < SIZE && m_isUsed[m_firstFreeObject])
			m_firstFreeObject++;

		return index;
	}

	template<typename T, unsigned int SIZE>
	void ObjectPool<T, SIZE>::FreeObject(unsigned int _index)
	{
#ifdef _DEBUG
		if (_index >= SIZE)
			return;
#endif

		m_isUsed[_index] = false;
		m_firstFreeObject = 0;

		while (m_firstFreeObject < SIZE && m_isUsed[m_firstFreeObject])
			m_firstFreeObject++;
	}

	template<typename T, unsigned int SIZE>
	void ObjectPool<T, SIZE>::FreeObject(T* & _item)
	{
		unsigned int i = 0;
		m_firstFreeObject = 0;

#ifdef _DEBUG
		while (i < SIZE && m_objects[i] != _item && m_isUsed[i])
			i++;

		if (i == SIZE)
			return;
#else
		while (m_objects[i] != _item && m_isUsed[i])
			i++;
#endif

		m_firstFreeObject = i;

		while (m_objects[i] != _item)
			i++;

		m_isUsed[i] = false;
		_item = nullptr;
	}

	template<typename T, unsigned int SIZE>
	void ObjectPool<T, SIZE>::FreeAllObjects()
	{
		for (unsigned int i = 0; i < SIZE; i++)
			m_isUsed[i] = false;

		m_firstFreeObject = 0;
	}

	template<typename T, unsigned int SIZE>
	T * ObjectPool<T, SIZE>::operator[](unsigned int _index)
	{
#ifdef _DEBUG
		if (_index >= SIZE)
			return nullptr;

		if(m_isUsed[_index])
			return m_objects[_index];

		return nullptr;
#else #ifdef _RELEASE
		return m_objects[_index];
#endif

	}
}