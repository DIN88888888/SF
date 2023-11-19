#include "TKeyboard.h"
TKeyboard::TKeyboard()
{
}
TKeyboard::TKeyboard(string name, float weight, float price, string MainHand, int keyCount, string KeyType, string info) :TController(name, weight, price, MainHand,info), _KeyCount(keyCount), _KeyType(KeyType){};
TKeyboard::~TKeyboard() {}
string TKeyboard::GetDescription()
{
	return TController::GetDescription() + TKeyboard::GetSelfDescription();
}
string TKeyboard::GetSelfDescription()
{
	return "Key count   : " + to_string(_KeyCount) + '\n' +
	       "Key type    : " + _KeyType + '\n';
}
string TKeyboard::GetShortDescription()
{
	return "Keybd " + getName();
}