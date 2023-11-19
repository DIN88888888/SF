#pragma once
#include <iostream>
#include <string>
#include "IItem.h"
using namespace std;

class TStorage
{
public:
	TStorage();
	~TStorage();
	void AddItem(IItem *it);
	void showList();
	void showInfo(int itemNum);
private:
	IItem** items;
	int icount;
};

