#include <iostream>
#include "intArray.h"

using namespace std;
void printArray(intArray &arr)
{
    for (int i = 0; i < arr.length(); ++i)
    {
        cout << " " << arr[i];
    }
}
void printState(intArray& a, intArray& b)
{
    cout << "A={";
    printArray(a);
    cout << " }" << "L=" << a.length() <<endl;
    cout << "B={";
    printArray(b);
    cout << " }" << "L=" << b.length() << endl;
}

void test_Good(intArray& a, intArray& b, int step)
{
    system("cls");
    cout << "State:" << step << endl;
    cout << "Current array state:" << endl;
    printState(a,b);
    cout << endl<<endl<<"--->>  ";
    int pos = 0;
    switch (step)
    {
    case 0:
        cout << "a.insert(b, 5);" << endl;
        a.insert(b, 5);
        break;
    case 1:
        cout << "a.insert(b);" << endl;
        a.insert(b);
        break;
    case 2:
        cout << "a.insert(b,a.length());" << endl;
        a.insert(b,a.length());
        break;
    case 3:        
        cout << "Delete all 11" << endl;
        while (a.find(pos, 11))        
            a.del(pos);        
        break;
    case 4:
        cout << "a.add(b.subarr(1, 3));" << endl;
        a.add(b.subarr(1, 3));
        break;
    case 5:
        cout << "a.resize(8);" << endl;
        a.resize(8);
        break;
    case 6:
        cout << "a.resize(10);" << endl;
        a.resize(10);
        break;
    case 7:        
        cout <<"a[5] = 25" << endl;
        a[5] = 25;
        break;
    case 8:
        cout << "b={4 4 4 4 4 }; a.copy(b, 4);" << endl;
        for (pos = 0; pos < b.length(); ++pos)b[pos] = 4;
        a.copy(b,4);
        break;
    case 9:
        cout << "a = b;" << endl;
        a = b;
        break;
    default:
        cout << "o_O" << endl;
        break;
    }
    cout << endl<<endl<<"result array state:" << endl;
    printState(a, b);
}
void test_bad(intArray& a, intArray& b, int step)
{
    system("cls");
    cout << "State:" << step << endl;
    cout << "Current array state:" << endl;
    printState(a, b);
    cout << endl << endl << "--->>  ";
    int pos = 0;
    switch (step)
    {
    case 0:
        cout << "a.insert(10 ,nullptr, 5);" << endl;
        a.insert(10 ,nullptr, 5);
        break;
    case 1:
        cout << "a.insert(3,-2);" << endl;
        a.insert(3,-2);
        break;
    case 2:
        cout << "a.insert(b, 150);" << endl;
        a.insert(b, 150);
        break;
    case 3:
        cout << "Delete all 2 with errors" << endl;
        while (a.find(pos, 2))
            a.del(pos+3);
        break;
    case 4:
        cout << "a.add(b.subarr(1, -3));" << endl;
        a.add(b.subarr(1, -3));
        break;
    case 5:
        cout << "a.resize(-8);" << endl;
        a.resize(-8);
        break;
    case 6:
        cout << "a.del(0,3000);" << endl;
        a.del(0,3000);
        break;
    case 7:
        cout << "a[-5] = 25" << endl;
        a[-5] = 25;
        break;
    case 8:
        cout << "b={4 4 4 4 4 }; a.copy(b, -4);" << endl;
        for (pos = 0; pos < b.length(); ++pos)b[pos] = 4;
        a.copy(b, -4);
        break;
    case 9:
        cout << "a = move(b);" << endl;
        a = move(b);
        break;
    default:
        cout << "o_O" << endl;
        break;
    }
    cout << endl << endl << "result array state:" << endl;
    printState(a, b);
}

int main()
{
    int da[] = { 1,1,1,1,1,2,2,2,2,2 };
    int db[] = { 9,11,11,11,9 };        
    int com = 1;    

    while (com)
    {
        intArray a(10, da);
        intArray b(5, db);

        system("cls");
        cout << "Select test operation:"<<endl<<"  1.  Test good"<<endl<< "  2.  Test exceptions"<<endl<<" enter (1,2 or 0 for exit):";
        cin >> com;
        if (com)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            for (int i = 0; i < 9; ++i)
            {
                try
                {
                    if (com == 1) test_Good(a, b, i);
                    if (com == 2) test_bad(a, b, i);
                }
                catch (out_of_range e)
                {
                    cout << "ERROR:  " << e.what();
                }
                catch (bad_typeid  e)
                {
                    cout << "ERROR:  nullptr " << e.what();
                }
                catch (bad_array_new_length e)
                {
                    cout << "ERROR:  " << e.what();
                }

                cout << endl<<endl<<"     <press enter>";
                
                cin.get();
            }
        }

    }
    cout << endl <<endl <<"constructors-destructors = " << intArray::MEMTEST << endl << endl << endl << endl;
}
