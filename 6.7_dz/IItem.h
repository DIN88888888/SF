#pragma once
#include <string>
using namespace std;

class IItem
{
public:
	IItem() {};
	virtual string GetDescription() = 0;
	virtual string GetSelfDescription() =0;
	virtual string GetShortDescription() =0;
	virtual ~IItem() {};
};

