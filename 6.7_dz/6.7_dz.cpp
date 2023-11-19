#include <iostream>
#include <string>

#include "IItem.h"
#include "TItem.h"

#include "TDisplay.h"
#include "TController.h"
#include "TPCBase.h"

#include "TMice.h"
#include "TKeyboard.h"
#include "TMonitor.h"
#include "TPCBlock.h"

#include "TNotebook.h"

#include "TStorage.h"


using namespace std;
int main()
{
    TStorage s;

    s.AddItem(new TMice("Speedy 2",0.1f,1200.f,"right",1200));
    s.AddItem(new TMice("Razor 7000", 0.2f, 3200.f, "any", 1200, "wifi, rgb lights"));
    
    s.AddItem(new TKeyboard("Cors A100", 0.5f, 7200.f, "any",112,"Cherry brown"));
    s.AddItem(new TKeyboard("BeshtauWBR", 0.35f, 17200.f, "russian", 227, "exists","for russia only"));

    s.AddItem(new TMonitor("Beshtau1.0", 7.15f, 25200.f,32.f,false,"stand, w120x120", "for russia only"));
    s.AddItem(new TMonitor("LG K24352", 4.67f, 45200.f, 27.5f, false, "stand, w80x80"));

    s.AddItem(new TPCBlock("HP400", 8.37f, 57200.f, 3.5f, 16000 ,6, "Mid tower"));
    s.AddItem(new TPCBlock("Lenovo K5", 3.1f, 47200.f, 2.7f, 8000, 4, "micro atx"));
    s.AddItem(new TPCBlock("HP800", 7.19f, 157200.f, 4.1f, 32000, 16, "full tower","after repair"));

    s.AddItem(new TNotebook("asis G44", 2.f, 97200.f, 16.f, 2.1f, 4000, 4, false,true));
    s.AddItem(new TNotebook("Rwe V1.0", 5.f, 187200.f, 21.f, 3.1f, 12000, 28, true, false,"special port D234 x4"));

    int com = -1;

    while (com)
    {
        system("cls");
        s.showList();       
        cout << "Enter number to wach or \"e\": for exit: ";
        cin >> com;
        if (com)
        {
            system("cls");
            s.showInfo(com);
        }
        cin.clear();
        cin.ignore(1000, '\n');
    }
   
}