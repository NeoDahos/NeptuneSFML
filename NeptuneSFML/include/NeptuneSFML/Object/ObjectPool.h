#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

namespace nep
{
	template <typename T, unsigned int SIZE>
	class ObjectPool
	{
	public:
		ObjectPool();
		~ObjectPool();

		T* GetObject();
		unsigned int ReserveObject();
		void FreeObject(unsigned int _index);
		void FreeObject(T* & _item);
		void FreeAllObjects();

		T* operator[](unsigned int _index);

	private:
		T* m_objects[SIZE];
		unsigned int m_firstFreeObject;
		bool m_isUsed[SIZE];
	};
}

#include <NeptuneSFML\Object\ObjectPool.inl>

#endif // OBJECT_POOL_H