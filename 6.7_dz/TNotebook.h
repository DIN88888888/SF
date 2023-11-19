#pragma once
#include "TPCBase.h"
#include "TDisplay.h"
#include <string>
using namespace std;

class TNotebook final :
    public TPCBase, TDisplay
{
public:
    TNotebook();
    TNotebook(string name, float weight, float price, float Diag, float mhz, int ram, int usbCount, bool fk, bool fm, string info = "");
    ~TNotebook();
    string GetDescription() override;
    string GetSelfDescription() override;
    string GetShortDescription() override;
private:
    bool _fullkeyboard;
    bool _flatmode;    
};

