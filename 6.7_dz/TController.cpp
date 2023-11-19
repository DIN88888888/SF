#include "TController.h"
TController::TController()
{
}
TController::TController(string name, float weight, float price, string MainHand, string info) :TItem(name, weight, price, info), _MainHand(MainHand) {};
TController::~TController() {}
string TController::GetDescription()
{
	return TItem::GetDescription() + TController::GetSelfDescription();
}
string TController::GetSelfDescription()
{
	return "Main hand   : " + _MainHand + '\n';
}
string TController::GetShortDescription()
{
	return "Cont " + getName();
}