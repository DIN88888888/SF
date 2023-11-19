#pragma once
#include "TItem.h"
#include <string>
using namespace std;

class TController  :
    public TItem
{
public:
    TController();
    TController(string name, float weight, float price, string MainHand, string info = "");
    ~TController();
    string GetDescription() override;
    string GetSelfDescription() override;
    string GetShortDescription() override;
private:
    string _MainHand;
};

