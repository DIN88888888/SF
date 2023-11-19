#pragma once
#include "TController.h"
#include <string>
using namespace std;

class TMice final :
    public TController
{
public:
    TMice();
    TMice(string name, float weight, float price, string MainHand, int DPI, string info = "");
    ~TMice();
    string GetDescription() override;
    string GetSelfDescription() override;
    string GetShortDescription() override;
private:
    int _dpi;
};

