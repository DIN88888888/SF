// забыл освободить память... лох...
#include <iostream>
using namespace std;
int main()
{
    char op = '\n';

    char *str = new char[1];
    str[0] = '\0';
    char *tmpstr = nullptr;
    int strl = 1;

    int x = 0;

    char c;

    bool formatC = false;


    cout << "Enter comand like \"O MyText X\" where O - operation c/d, X - integer number" << endl;
    cout << "you can split string on different lines, first space in text will end string" << endl;
    cout << "use only a-z,A-Z chars for text. if you type any undeclared char programm will format C:\\" << endl;


    cin >> op;
    if (op != 'c' && op != 'd')
    {
        cout << endl << "operation \""<<op<<"\" not suported";
        return 0;
    }
    c = cin.get();
    while(true)
    {
        c = cin.get();
        if (c == ' ' || c=='\t')
        {
            break;
        }
        else if(c != '\n')
        {            
            formatC = !((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
            if (formatC)break;

            strl++;
            tmpstr = new char[strl];
            for (int i = 0; i < strl; i++)
            {
                tmpstr[i] = str[i];
            }
            tmpstr[strl - 2] = c;
            tmpstr[strl - 1] = '\0';
            delete[] str;
            str = tmpstr;
            tmpstr = nullptr;            
        }
    }

    cin >> x;
    if (formatC)
    {
        cout << endl << endl << "format C:\\ started... 0%";
        //to do
        return 0;
    }

    if (op == 'd')
        x *= -1;

    int i = 0;
    while (str[i] != '\0')
    {
        int v;
        v = str[i];

        if (v <= 'Z')
            c = 'A';
        else
            c = 'a';
        
        v = (26 + (v - c + x)) % 26 + c;             

        str[i] = v;
        i++;
    }

    cout << endl<<"result: " << str << endl << endl;

    
    




}
