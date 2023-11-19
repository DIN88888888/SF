#include "TPCBase.h"
TPCBase::TPCBase()
{
}
TPCBase::TPCBase(string name, float weight, float price, float mhz, int ram, int usbCount, string info) :TItem(name, weight, price, info), _mhz(mhz), _ram(ram), _UsbCount(usbCount) {};
TPCBase::TPCBase(float mhz, int ram, int usbCount) :_mhz(mhz), _ram(ram), _UsbCount(usbCount) {};
TPCBase::~TPCBase() {}
string TPCBase::GetDescription()
{
	return TItem::GetDescription() + TPCBase::GetSelfDescription();
}
string TPCBase::GetSelfDescription()
{
	return "CPU mhz     : " + to_string(_mhz) + '\n' +
		   "RAM         : " + to_string(_ram) + '\n' +
		   "USB count   : " + to_string(_UsbCount) + '\n';
}
string TPCBase::GetShortDescription()
{
	return "CPU " + getName();
}