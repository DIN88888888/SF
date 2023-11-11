// 1_10_dz.cpp 
//

#include <iostream>
using namespace std;

int main()
{
    bool init = true;

    while (init)
    {
        long long a, b;
        cout << "enter numbers like \"1 2\":" << endl;
        cin >> a >> b;//+  -  *  /  &  |  ^
        cout << "a + b = " << a + b << endl;
        cout << "a - b = " << a - b << endl;
        cout << "a * b = " << a * b << endl;
        cout << "a / b = " <<  ((long double)a / (long double)b) << endl;
        cout << "a & b = " << (a & b) << endl;
        cout << "a | b = " << (a | b) << endl;
        cout << "a ^ b = " << (a ^ b) << endl;

        cout << "again?(y/n)" << endl<< ">";        

        char q;
        cin >> q;

        init = q=='y';
    }
}

