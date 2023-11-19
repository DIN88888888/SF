#include "TDisplay.h"

TDisplay::TDisplay()
{
}
TDisplay::TDisplay(string name, float weight, float price, float Diag, string info) :TItem(name, weight, price, info), _Diagon(Diag) {};
TDisplay::TDisplay(float Diag) :_Diagon(Diag) {};
TDisplay::~TDisplay(){}

string TDisplay::GetDescription()
{
	return TItem::GetDescription() + TDisplay::GetSelfDescription();
}
string TDisplay::GetSelfDescription()
{
	return "Diagonal    : " + to_string(_Diagon) + '\n';
}
string TDisplay::GetShortDescription()
{
	return "Disp " + getName();
}