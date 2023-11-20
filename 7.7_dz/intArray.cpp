#include "intArray.h"
int intArray::MEMTEST = 0;

intArray::intArray(intArray&& o) :intArray()
{
	swap(items,o.items);
	swap(icount, o.icount);
}
intArray::intArray(const intArray& o) :intArray()
{
	resize(o.icount);
	copy(o);
}
intArray::~intArray()
{
	MEMTEST -= 1;
	if (items)
		delete[] items;
}

void intArray::resize(int nSize)
{	
	//nsize<0
	if (nSize < 0)throw bad_array_new_length();


	if (nSize == icount)
		return;
	if (nSize == 0)
	{
		if (items)
			delete[] items;
		items = nullptr;
		icount = 0;
		return;
	}

	int* nit = new int[nSize];
	if (items)
	{
		memcpy(nit, items, min(icount, nSize) * SOFI);
		delete[] items;
	}
	if(nSize>icount)
		memset(nit+icount, 0, (nSize-icount)*SOFI);
	items = nit;
	icount = nSize;
}

void intArray::insert(int size, int* arr, int before)
{	
	if (size && !arr) throw bad_typeid();
	if (size < 0)  throw out_of_range("insert failure:  size < 0");
	if (before<0)  throw out_of_range("insert failure: before<0");
	if (before>icount)throw out_of_range("insert failure: before>length");

	if (!size)
		return;
	int* nit = new int[size+icount];
	if (items)
	{
		memcpy(nit, items, before * SOFI);		//left
		memcpy(nit+before+size, items+before, (icount-before) * SOFI);//right if exists
		delete[] items;
	}
	memcpy(nit+before, arr, size * SOFI);
	icount += size;
	items = nit;
}

void intArray::copy(int size, const int* a, int startpos)
{
	//startpos>icount
	//startpos<0
	//size<0
	//size>0 and a==nullptr
	if (startpos > icount)throw out_of_range("copy failure: startpos > length");
	if (startpos < 0)throw out_of_range("copy failure: startpos < 0");
	if (size<0)throw out_of_range("copy failure: size<0");
	if (size && !a) throw bad_typeid();

	int* nit = items;
	if (startpos + size > icount)
	{
		nit = new int[startpos + size];		
		if (items)
		{
			memcpy(nit, items, startpos * SOFI);//it is OK if 0		
			delete[] items;
		}
		items = nit;
		icount = startpos + size;
	}
	memcpy(nit+startpos, a, size * SOFI);	
}

void intArray::del(int start, int count)
{	
	if (start < 0)throw out_of_range("del failure: start < 0");
	if (start >= icount)throw out_of_range("del failure: start > length");
	if (count<0)throw out_of_range("del failure: count < 0");
	if (start+count>icount)throw out_of_range("del failure: deleting outside end of array");
	

	if (!count)
		return;
	if (count == icount)
	{
		resize(0);
		return;
	}
	int* nit = new int[icount-count];	
	memcpy(nit, items, start * SOFI);//icount>0 allwais, 0 is ok		
	memcpy(nit + start, items+start+count, (icount - start - count) * SOFI);

	delete[] items;
	icount -=count;
	items = nit;
}

int& intArray::operator [](int ind)
{
	//icount<=ind<0
	if (ind < 0 || ind >= icount)throw out_of_range("[] out of range");
	return items[ind];
}
void intArray::operator=(const intArray& o)
{	
	copy(o);
	resize(o.icount);
}
void intArray::operator=(intArray&& o)
{
	swap(items, o.items);
	swap(icount, o.icount);
}

bool intArray::find(int& result_ind, int target)
{
	for (result_ind = 0; result_ind < icount; ++result_ind)
		if (items[result_ind] == target) 
			return true;
	return false;
}
int intArray::length() const
{
	return icount;
}
intArray intArray::subarr(int start, int count) const
{
	//0>start
	//count+start>icount	;
	if (start < 0)throw out_of_range("subarr failure: start < 0");
	if (start+count>icount)throw out_of_range("subarr failure: start+count>length");
	return move(intArray(count, items + start));
}