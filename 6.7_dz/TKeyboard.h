#pragma once
#include "TController.h"
#include <string>
using namespace std;

class TKeyboard final:
    public TController
{
public:
    TKeyboard();
    TKeyboard(string name, float weight, float price, string MainHand, int keyCount, string KeyType, string info = "");
    ~TKeyboard();
    string GetDescription() override;
    string GetSelfDescription() override;
    string GetShortDescription() override;
private:
    int _KeyCount;
    string _KeyType;
};

