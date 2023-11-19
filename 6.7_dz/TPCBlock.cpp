#include "TPCBlock.h"
TPCBlock::TPCBlock()
{
}
TPCBlock::TPCBlock(string name, float weight, float price, float mhz, int ram, int usbCount, string size, string info) :TItem(name, weight, price, info), TPCBase( mhz ,ram ,usbCount), _size(size) {};
TPCBlock::~TPCBlock() {}
string TPCBlock::GetDescription()
{
	return TPCBase::GetDescription() + TPCBlock::GetSelfDescription();
}
string TPCBlock::GetSelfDescription()
{
	return "Size        : " + _size + '\n';
}
string TPCBlock::GetShortDescription()
{
	return "PC " + getName();
}
