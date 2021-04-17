#pragma once

#include <iostream>

#define MAX_STACK_SIZE 100000000
using namespace std;


template <class T>
class TStack
{
	int Size, MaxSize;
	T* arr;
public:
	TStack(int _MaxSize = 10); 
	TStack(const TStack<T>& m);
	~TStack();
	TStack& operator=(const TStack& m); 
	T& operator[](int pos);
	bool operator==(const TStack& st) const;  
	bool operator!=(const TStack& st) const;

	int GetMaxSize();
	int GetSize();
	bool Empty();
	bool Full();
	void Clear();
	void Push(T a);
	T Pop();
	T Top();

	friend istream& operator>>(istream& in, TStack& st)
	{
		in >> st.Size;
		if (st.Size > st.MaxSize)
			throw "Size > MaxSize";
		for (int i = 0; i < st.Size; i++)
			in >> st.arr[i];
		return in;
	}

	friend ostream& operator<<(ostream & out, const TStack & st)
	{
		for (int i = 0; i < st.Size; i++)
			out << st.arr[i] << ' ';
		return out;
	}
};

template <class T>
TStack<T>::TStack(int _MaxSize)
{
	if (_MaxSize > MAX_STACK_SIZE || _MaxSize < 0)
		throw "incorrect number";
	MaxSize = _MaxSize;
	arr = new T[MaxSize];
	Size = 0;
}

template <class T>
TStack<T>::TStack(const TStack<T> & m)
{
	MaxSize = m.MaxSize;
	arr = new T[MaxSize];
	Size = m.Size;
	for (int i = 0; i < Size; i++)
	{
		arr[i] = m.arr[i];
	}
}

template <class T>
TStack<T>::~TStack()
{
	delete[] arr;
}

template <class T>
TStack<T>& TStack<T>::operator=(const TStack & m)
{
	if (MaxSize != m.MaxSize)
	{
		delete[] arr;
		MaxSize = m.MaxSize;
		arr = new T[MaxSize];
	}
	Size = m.Size;
	for (int i = 0; i < Size; i++)
	{
		arr[i] = m.arr[i];
	}
	return *this;
}

template <class T>
T& TStack<T>::operator[](int pos)
{
	if (pos < 0)
		throw "Negative index";
	if (pos > Size)
		throw "Too large index";
	return arr[pos];
}

template <class T>
bool TStack<T>::operator==(const TStack & st) const
{
	if (Size != st.Size)
		return 0;
	for (int i = 0; i < Size; i++)
	{
		if (arr[i] != st.arr[i])
			return 0;
	}
	return 1;
}

template <class T>
bool TStack<T>::operator!=(const TStack & st) const
{
	return !(*this == st);
}

template <class T>
int TStack<T>::GetMaxSize()
{
	return MaxSize;
}

template <class T>
int TStack<T>::GetSize()
{
	return Size;
}

template <class T>
bool TStack<T>::Empty()
{
	if (Size == 0)
		return true;
	return false;
}

template <class T>
bool TStack<T>::Full()
{
	if (Size == MaxSize)
		return true;
	return false;
}

template <class T>
void TStack<T>::Clear()
{
	Size = 0;
}

template <class T>
void TStack<T>::Push(T a)
{
	if (Full())
		throw MaxSize;
	arr[Size] = a;
	Size++;
}

template <class T>
T TStack<T>::Pop()
{
	if (Empty())
		throw 0;
	Size--;
	return arr[Size];
}

template <class T>
T TStack<T>::Top()
{
	if (Empty())
		throw 0;
	return arr[Size - 1];
}