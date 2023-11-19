#include "TNotebook.h"
TNotebook::TNotebook()
{
}
TNotebook::TNotebook(string name, float weight, float price, float Diag, float mhz, int ram, int usbCount, bool fk, bool fm, string info) :TItem(name, weight, price, info),TDisplay(Diag), TPCBase(mhz,ram,usbCount), _fullkeyboard(fk), _flatmode(fm) {};
TNotebook::~TNotebook() {}
string TNotebook::GetDescription()
{
	return TDisplay::GetDescription() + TPCBase::GetSelfDescription() + TNotebook::GetSelfDescription();
}
string TNotebook::GetSelfDescription()
{
	return "Full keybd  : " + string((_fullkeyboard) ? "True" : "False") + '\n' +
		"Flat mode   : " + string((_flatmode) ? "True" : "False") + '\n';
}
string TNotebook::GetShortDescription()
{
	return "Note " + getName();
}