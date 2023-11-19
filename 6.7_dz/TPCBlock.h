#pragma once
#include "TPCBase.h"
#include <string>
using namespace std;

class TPCBlock final :
    public TPCBase
{
public:
    TPCBlock();
    TPCBlock(string name, float weight, float price, float mhz, int ram, int usbCount, string size, string info = "");
    ~TPCBlock();
    string GetDescription() override;
    string GetSelfDescription() override;
    string GetShortDescription() override;
private:
    string _size;
};

