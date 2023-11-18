#include <iostream>
#include "calculate.h"
using namespace std;

int main()
{
    char doit = 'y';
    while (doit == 'y')
    {
        cout << "Enter equasion like \"A o [B]\" where A,B - numbers, o - one of \"+-*/^!\"\n";
        float a = 0, b = 0;
        char c = '\0';

        cin >> a >> c;
        if (c != '!') cin >> b;

        switch (c)
        {
        case '+':
            cout << "summ = " << cSum(a, b);
            break;

        case '-':
            cout << "diff = " << cDif(a, b);
            break;

        case '*':
            cout << "prod = " << cPro(a, b);
            break;

        case '/':
            cout << "div = " << cDiv(a, b);
            break;

        case '^':
            if (b >= 0.0f)
                cout << a << "^" << b << " = " << cPow(a, (short)b);            
            else            
                cout << "negative values are not alowed.";            
            break;

        case '!':        
            cout << a << "! = " << cFak((short)a);
            break;

        default:
            cout << "type one of alowed operations.";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        
        cout << endl<<"repeat (y/n) ?:";
        cin>>doit;
    }
}
