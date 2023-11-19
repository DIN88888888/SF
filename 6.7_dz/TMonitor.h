#pragma once
#include "TDisplay.h"
#include <string>
using namespace std;

class TMonitor final :
    public TDisplay
{
public:
    TMonitor();
    TMonitor(string name, float weight, float price, float Diag, bool s3d, string Mount, string info = "");
    ~TMonitor();
    string GetDescription() override;
    string GetSelfDescription() override;
    string GetShortDescription() override;
private:
    bool _3dsupport;
    string _MountType;
};

