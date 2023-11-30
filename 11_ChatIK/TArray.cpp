#include "TArray.h"
using namespace std;

//TArray
template<typename T> void TArray<T>::add(T* item)
{
	count += 1;
	items = (T**)realloc(items, sizeof(T) * count);
	items[count - 1] = item;	
}
template<typename T> T* TArray<T>::operator[](int ind)
{
	if (ind < 0 || ind >= count) throw ex_arr_out_of_range();
	return items[ind];
}
template<typename T> T* TArray<T>::eject(int ind)
{
	if (ind < 0 || ind >= count) throw ex_arr_out_of_range();
	T* res = items[ind];
	items[ind] = items[count - 1];
	return res;
}
template<typename T>TArray<T>::~TArray()
{
	for (int i; i < count; i++)
	{
		delete (items + i);
	}
}
template<typename T>int TArray<T>::len()
{
	return count;
}
