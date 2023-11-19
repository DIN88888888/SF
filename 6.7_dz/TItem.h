#pragma once
#include <string>
#include "IItem.h"
using namespace std;

class TItem :
	virtual public IItem
{
public:	
	TItem();
	TItem(string name, float weight, float price, string info);
	~TItem()override;

	string GetDescription() override;
	string GetSelfDescription() override;
	string GetShortDescription() override;
	virtual void setInfo(string info);
	string getName();
public:
	string _name;
	float _weight;
	float _price;
	string* _specialInfo;
};

