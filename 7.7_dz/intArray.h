#pragma once

#include <memory.h>
#include <algorithm>
#include <typeinfo>
#include <stdexcept>

using namespace std;
const int SOFI = sizeof(int);


class intArray
{
public:
	static int MEMTEST;
	intArray() :icount(0), items(nullptr)
		{ MEMTEST += 1;}	
	intArray(int Size) :intArray() 
		{ resize(Size);}
	intArray(int Size, const int* o) :intArray() 
		{ copy(Size, o);}
	intArray(const intArray& o);	
	intArray(intArray&& o);

	~intArray();

	void resize(int nSize);

	void insert(int size, int* arr, int before = 0);
	void insert(int v, int before = 0)
			{ insert(1, &v, before); }
	void insert(const intArray& o, int before = 0)
			{ insert(o.icount, o.items, before);}

	void add(int size, int* arr) 
			{ insert(size,arr,icount); }
	void add(int v) 
			{ insert(1, &v, icount); }
	void add(const intArray& o)
			{ insert(o.icount, o.items, icount);}

	void copy(int size, const int* a, int startpos = 0);
	void copy(const intArray& o, int startpos = 0)
			{ copy(o.icount, o.items, startpos); }
	void copy(int size, const intArray& o, int startpos = 0)
			{ copy(size, o.items, startpos); }

	void del(int start, int count = 1);

	int& operator [](int ind);
	void operator=(const intArray& o);
	void operator=(intArray&& o);

	bool find(int& ind, int target);

	intArray subarr(int start, int count) const;
		
	int length() const;
private:

	int icount;
	int* items;
};

