#pragma once
#include "TItem.h"
#include <string>
using namespace std;

class TDisplay  :
    public virtual TItem
{
public:
    TDisplay();
    TDisplay(string name, float weight, float price, float Diag, string info = "");
    TDisplay(float Diag);
    ~TDisplay();
    string GetDescription() override;
    string GetSelfDescription() override;
    string GetShortDescription() override;    
private:
    float _Diagon;
};

