#include "TMice.h"
TMice::TMice()
{
}
TMice::TMice(string name, float weight, float price, string MainHand, int DPI, string info) :TController(name, weight, price, MainHand, info), _dpi(DPI) {};
TMice::~TMice() {}
string TMice::GetDescription()
{
	return TController::GetDescription() + TMice::GetSelfDescription();
}
string TMice::GetSelfDescription()
{
	return "DPI         : " + _dpi + '\n';
}
string TMice::GetShortDescription()
{
	return "Mice " + getName();
}