﻿//
// Created by kamil on 3/30/17.
// 
//

#ifndef KROLIB_VECTOR_H
#define KROLIB_VECTOR_H
#include "Exception.h"
#include <cstddef>
#include <cmath>
enum SizeManagement {
	STATIC, DYNAMIC
};
//__________________________________Vector header______________________________________
template<class Type>
class Vector {
	class Wrapper;

public:
	class Iterator;
	Vector();
	Vector(const Vector&other);
	/*explicit*/ Vector(size_t initSize, SizeManagement = STATIC);
	~Vector();
	//Wrapper operator[](size_t index);
	Type& operator[](size_t index);
	Vector<Type>&operator=(const Vector<Type>&other);
	Type  getLast();
	size_t getTailIndex() const;
	void pushLast(Type object);
	size_t getSize() const;
	Iterator begin()const;
	Iterator end()const;

private:
	void resizeArray();
	SizeManagement sizeManagement;
	Type *array;
	size_t size;
	Type *lastPtr;
	size_t tailIndex;
};

//___________________________________Wrapper header_______________________________________
template <class Type>
class Vector<Type>::Wrapper {
public:
	Wrapper(Vector<Type> *arr, size_t index) : index(index), arr(arr) {}
	//operator Type() const;
	/*explicit*/ operator Type() const;
	Wrapper& operator=(Type newObject);
private:
	size_t index;
	Vector<Type> *arr;
};

//___________________________________Iterator header______________________________________
template <class Type>
class Vector<Type>::Iterator
{
	typedef int OffsetType;
public:
	Iterator();
	Iterator(const Iterator&other);
	~Iterator();

	Type operator*()const;
	//Type* operator->()const;
	Iterator& operator++();
	Iterator operator++(int);
	Iterator& operator--();
	Iterator operator--(int);
	Iterator& operator+=(OffsetType offset);
	Iterator operator+(OffsetType offset)const;
	Iterator& operator-=(OffsetType offset);
	Iterator operator-(OffsetType offset)const;
	bool operator==(const Iterator&other)const;
	bool operator!=(const Iterator&other)const;
	bool operator>(const Iterator&other)const;
	bool operator>=(const Iterator&other)const;
	bool operator<(const Iterator&other)const;
	bool operator<=(const Iterator&other)const;
private:
	/*explicit*/ Iterator(Type*ptr);
	Type*ptr;
	friend Vector;
};




//__________________________________Vector implementation___________________________________
template<class Type>
Vector<Type>::Vector()
{
	size = 1;
	sizeManagement = DYNAMIC;
	tailIndex = 0;
	array = new Type[size]();
	lastPtr = array + size - 1;
}

template <class Type>
Vector<Type>::Vector(const Vector& other) : sizeManagement(other.sizeManagement), array(new Type[other.size]), size(other.size), tailIndex(other.tailIndex)
{
	for (int i = 0; i < size; ++i)
		array[i] = other.array[i];
	lastPtr = array + size - 1;
}

template<class Type>
Vector<Type>::Vector(size_t size, SizeManagement sizeManagement) :tailIndex(0) {
	array = new Type[size]();
	this->size = size;
	lastPtr = array + size - 1;
	this->sizeManagement = sizeManagement;
}
template<class Type>
Vector<Type>::~Vector() {
	delete[] array;
}

//template<class Type>
//typename Vector<Type>::Wrapper Vector<Type>::operator[](size_t index) {
//      return Wrapper(this, index);
//}

template<class Type>
Type& Vector<Type>::operator[](size_t index) {

	if (index < size && index >= 0)
		return *(array + index);
	if (sizeManagement == DYNAMIC) {
		resizeArray();
		return *(array + index);
	}
	throw OutOfRangeException();
}

template <class Type>
Vector<Type>& Vector<Type>::operator=(const Vector<Type>& other)
{
	for (Iterator it = other.begin(); it != other.end(); ++it)
	{
		this->pushLast(*it);
	}
	return *this;
}

template <class Type>
Type Vector<Type>::getLast()
{
	return *lastPtr;
}

template <class Type>
size_t Vector<Type>::getTailIndex() const
{
	return tailIndex;
}

template <class Type>
void Vector<Type>::pushLast(Type object)
{
	if (tailIndex >= size - 1)
		this->resizeArray();
	this->array[tailIndex] = object;
	tailIndex++;
	//operator[](tailIndex) = object;
}


template<class Type>
size_t Vector<Type>::getSize() const {
	return size;
}

template <class Type>
typename Vector<Type>::Iterator Vector<Type>::begin() const
{
	return Iterator(array);
}

template <class Type>
typename Vector<Type>::Iterator Vector<Type>::end() const
{
	return Iterator(&array[tailIndex]);
}

template<class Type>
void Vector<Type>::resizeArray() {
	size_t newSize;
	if (size > 0)
		if (size > 1000)
			newSize = size * 1.2;

		else
			newSize = size * 2;
	else
		newSize = 1;
	Type *newArray = new Type[newSize]();
	Type *oldPtr = array;
	Type *newPtr = newArray;
	for (int i = 0; i < size; ++i)
		*(newPtr++) = *(oldPtr++);
	delete[] array;
	array = newArray;
	size = newSize;
	lastPtr = array + size - 1;

}


//_________________________________Wrapper implementation_______________________________
//template <class Type>
//Vector<Type>::Wrapper::operator Type() const
//{
//      if (index < arr->size && index >= 0)
//              return *(this->arr->array + index);
//      //throw OutOfRangeException();
//      return nullptr;
//}
template <class Type>
Vector<Type>::Wrapper::operator Type() const
{

	if (index < arr->size && index >= 0)
		return *(this->arr->array + index);
	if (arr->sizeManagement == STATIC)
		throw OutOfRangeException();
	while (index >= arr->size)arr->resizeArray();
	arr->tailIndex = index + 1;
	return *(this->arr->array + index);
}
template <class Type>
typename Vector<Type>::Wrapper& Vector<Type>::Wrapper::operator=(Type newObject)
{
	if (index < arr->size && index >= 0)
	{
		*(arr->array + index) = newObject;
		arr->tailIndex = index + 1;
		return *this;
	}
	if (arr->sizeManagement == STATIC || arr->size < 0)
		throw OutOfRangeException();
	while (index >= arr->size) arr->resizeArray();
	arr->array[index] = newObject;
	arr->tailIndex = index + 1;
	return *this;
}


//____________________________Iterator implementation___________________________________________
template <class Type>
Vector<Type>::Iterator::Iterator() :ptr(nullptr)
{
}

template <class Type>
Vector<Type>::Iterator::Iterator(const Iterator& other) : ptr(other.ptr)
{
}

template <class Type>
Vector<Type>::Iterator::~Iterator()
{
}

template <class Type>
Type Vector<Type>::Iterator::operator*() const
{
	if (!ptr)throw NullReferenceException();
	return *ptr;
}

template <class Type>
typename Vector<Type>::Iterator& Vector<Type>::Iterator::operator++()
{
	++ptr;
	return *this;
}

template <class Type>
typename Vector<Type>::Iterator Vector<Type>::Iterator::operator++(int)
{
	Iterator tmp = *this;
	++ptr;
	return tmp;
}

template <class Type>
typename Vector<Type>::Iterator& Vector<Type>::Iterator::operator--()
{
	--ptr;
	return *this;
}

template <class Type>
typename Vector<Type>::Iterator Vector<Type>::Iterator::operator--(int)
{
	Iterator tmp = *this;
	--ptr;
	return tmp;
}

template <class Type>
typename Vector<Type>::Iterator& Vector<Type>::Iterator::operator+=(OffsetType offset)
{
	ptr += offset;
	return *this;
}

template <class Type>
typename Vector<Type>::Iterator Vector<Type>::Iterator::operator+(OffsetType offset) const
{
	return Iterator(ptr + offset);
}

template <class Type>
typename Vector<Type>::Iterator& Vector<Type>::Iterator::operator-=(OffsetType offset)
{
	ptr -= offset;
	return *this;
}

template <class Type>
typename Vector<Type>::Iterator Vector<Type>::Iterator::operator-(OffsetType offset) const
{
	return Iterator(ptr - offset);
}

template <class Type>
bool Vector<Type>::Iterator::operator==(const Iterator& other) const
{
	return ptr == other.ptr;
}

template <class Type>
bool Vector<Type>::Iterator::operator!=(const Iterator& other) const
{
	return ptr != other.ptr;
}

template <class Type>
bool Vector<Type>::Iterator::operator>(const Iterator& other) const
{
	return ptr > other.ptr;
}

template <class Type>
bool Vector<Type>::Iterator::operator>=(const Iterator& other) const
{
	return ptr >= other.ptr;
}

template <class Type>
bool Vector<Type>::Iterator::operator<(const Iterator& other) const
{
	return ptr < other.ptr;
}

template <class Type>
bool Vector<Type>::Iterator::operator<=(const Iterator& other) const
{
	return ptr <= other.ptr;
}

template <class Type>
Vector<Type>::Iterator::Iterator(Type* ptr) : ptr(ptr)
{
}



#endif //KROLIB_VECTOR_H
