#pragma once
#include "Exceptions.h"
#include <memory>
using namespace std;

template<typename T>
class TArray
{
public:
	TArray() :items(nullptr), count(0) {};
	~TArray();
	void add(T* item);
	T* operator[](int ind);
	T* eject(int ind);
	int len();
private:
	T** items;
	int count;
};

