#include "TStorage.h"

TStorage::TStorage():items(nullptr),icount(0){}
TStorage::~TStorage()
{
	if (items)
	{
		for (int i = 0; i < icount; ++i)		
			delete items[i];
		delete[] items;
	}
}
void TStorage::AddItem(IItem *it)
{
	IItem** tmp;
	icount++;
	tmp = new IItem* [icount];

	for (int i = 0; i < icount - 1; ++i)	
		tmp[i] = items[i];
	tmp[icount - 1] = it;
	if (items)
		delete[] items;

	items = tmp;
}
void TStorage::showList()
{
	if (!icount)
	{
		cout << " 1. Sorry. Storage is empty"<< endl;
		return;
	}
	cout << "Storage contains:" << endl;
	for (int i = 0; i < icount; ++i)
	{
		cout << " " << (i + 1) << ". " << items[i]->GetShortDescription() << endl;
	}
}
void TStorage::showInfo(int itemNum)
{
	if (itemNum <= 0 || itemNum > icount)
	{
		cout << "Wrong item number" << endl << "     Press <Enter> to return." << endl;
		cin.get();
		return;
	}
	cout << items[itemNum - 1]->GetDescription() << endl << "     Press <Enter> to return." << endl;
	cin.get();
}