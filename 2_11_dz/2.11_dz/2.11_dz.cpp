#include <iostream>
using namespace std;
int main()
{
    cout << "Enter equasion like \"A o B\" where A,B - numbers, o - one of \"+-*/^!\"\n";
    float a = 0, b = 0;
    char c = '\0';

    cin >> a >> c;
    if(c!='!') cin>> b;

    int i = 0;
    float t = 0;
    switch (c)
    {
    case '+':
        cout << "summ = " << (a + b);
        break;
    case '-':
        cout << "diff = " << (a - b);
        break;
    case '*':
        cout << "prod = " << (a * b);
        break;
    case '/':
        ((b>0.0000001) || (b<-0.0000001))?(cout << "div = " << (a / b)) : (cout<<"cant divide on 0!");
        break;
    case '^':
        if (b > 0)
        {
            for (t = a, i = 1; i < b; i++)
                t *= a;
            cout << a << "^" << b << " = " << t;
        }
        else if(b==0)
        { 
            cout << a << "^" << b << " = " << 1;
        }
        else
        {
            cout << "negative values are not alowed.";
        }
        break;
    case '!':
        t = (int)a - a;
        if (t > 0.1 || t < -0.1)
        {
            cout << "enter integer value plz.";
        }
        else
        {
            for (t = 1, i = 2; i <= (int)a; i++)
                t *= i;
            cout << a << "! = " << t;
        }
        break;
    default:
        cout << "type one of alowed operations.";
    }        
    cout << endl;

}
