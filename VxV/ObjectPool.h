#pragma once
#include <vector>
#include <iostream>

template <class T>
class ObjectPool
{
private:
	std::vector<T*> m_pool;
	std::vector<T*> m_activeObjects;
	typedef memory_pool<ObjectPool, mempool_malloc<ObjectPool>> MemoryPool;
	static m_MemoryPool memP;

public:
	ObjectPool(int poolSize)
	{
		for (int i = 0; i < poolSize; i++) 
		{
			m_pool.push_back(new T());
		}
	}
	~ObjectPool()
	{
		for (T* obj : m_pool) 
		{
			delete obj;
		}
		m_pool.clear();
	}


	//Get an object from the pool
	T* GetObject() {
		for (T* obj : m_activeObjects) {
			// d�finir une m�thode de comparaison appropri�e pour les objets
			if (/* Condition pour v�rifier si l'objet est d�j� actif */) {
				return obj;
			}
		}
		if (m_pool.empty()) {
			// Si la pool est vide, cr�er un nouvel objet
			T* newObj = new T();
			m_activeObjects.push_back(newObj);
			return newObj;
		}
		else {
			// Sinon, r�utiliser un objet existant dans le pool
			T* obj = m_pool.back();
			m_pool.pop_back();
			m_activeObjects.push_back(obj);
			return obj;
		}
	}
};

