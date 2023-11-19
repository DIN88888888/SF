#pragma once
#include <iostream>
#include "TItem.h"
#include <string>
using namespace std;

class TPCBase  :
    public virtual TItem
{
public:
    TPCBase();
    TPCBase(string name, float weight, float price, float mhz, int ram, int usbCount, string info = "");
    TPCBase(float mhz, int ram, int usbCount);
    ~TPCBase();
    string GetDescription() override;
    string GetSelfDescription() override;
    string GetShortDescription() override;
private:
    float _mhz;
    int _ram;
    int _UsbCount;
};

