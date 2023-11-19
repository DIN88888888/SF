#include "TItem.h"

TItem::TItem() {};
TItem::TItem(string name, float weight, float price, string info = "") :_name(name), _weight(weight), _price(price), _specialInfo((info.length() > 0) ? new string(info) : nullptr) {};
TItem::~TItem()
{
	if (_specialInfo)delete _specialInfo;
}

string TItem::GetDescription()
{
	return TItem::GetSelfDescription();
}
string TItem::GetSelfDescription()
{
	return "Product name: " + _name + '\n' +
		"Weight      : " + to_string(_weight) + '\n' +
		"Price       : " + to_string(_price) + '\n' +
		((_specialInfo) ? "Info        : " + *_specialInfo + '\n' : "");
}
string TItem::GetShortDescription()
{
	return "Item " + _name; 
}
void TItem::setInfo(string info)
{
	_specialInfo = new string(info);
}
string TItem::getName()
{
	return _name;
}
