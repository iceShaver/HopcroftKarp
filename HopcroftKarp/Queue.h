#pragma once
#include "List.h"
#include <cstddef>
template <class Type>
class Queue : List<Type>
{
	typedef List<Type> base;
public:
	Queue();
	Type pop();
	Type peek();
	void push(Type);
	size_t GetSize();
	bool empty()const;
	~Queue();
};

template <class Type>
Queue<Type>::Queue()
{
	elementsNumber = 0;
	indexCounter = 0;
	firstPtr = nullptr;
	lastPtr = nullptr;
}

template <class Type>
Type Queue<Type>::pop()
{
	return List<Type>::PopFirst();
}

template <class Type>
Type Queue<Type>::peek()
{
	return List<Type>::GetFirst();
}

template <class Type>
void Queue<Type>::push(Type object)
{
	List<Type>::PushLast(object);
}

template <class Type>
size_t Queue<Type>::GetSize()
{
	return List<Type>::GetSize();
}

template <class Type>
bool Queue<Type>::empty() const
{
	return elementsNumber == 0;
}

template <class Type>
Queue<Type>::~Queue()
{
}
