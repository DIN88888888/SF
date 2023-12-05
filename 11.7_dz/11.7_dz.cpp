#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

using namespace std;
#define SFP "source.txt"
#define NCOUNT 2000000
#define NREAD 10000

void generate() {    
    fstream f(SFP,ios::out);
    if (f.is_open()) {
        for (int i = 0; i < NCOUNT; i++) {
            f << rand();
            if(i<NCOUNT-1)f<< " ";
        }
        f.close();
    }
}
void InsertEl(int* arr, int n, int el) {
    int i = 0;
    for (; i < n; i++)
        if (arr[i] > el) break;
    for (int j = n; j > i; j--)
        arr[j] = arr[j-1];
    arr[i] = el;
}
int ReadPart(fstream &f,int *arr) {
    int i = 0;
    for (i = 0; i < NREAD; i++) {
        if (f.eof()) break;
        int nEl;
        f >> nEl;
        InsertEl(arr,i,nEl);
    }
    return i;
}
string Merge(int* arr, int n) {
    static int step = 0;
    step = (step + 1) % 2;
    string rf = "step" + to_string((step + 1) % 2) + ".txt";

    fstream src("step" + to_string(step) + ".txt", ios::in);
    fstream dst(rf, ios::out);
    
    if (!dst.is_open()) {
        cout << "dsterr" << endl;
        return "";
    }
    int i = 0;

    if (!src.is_open()) {
        while (i < n) {
            dst << arr[i++];
            if (i < n)dst << " ";
        }
        return rf;
    }
    
    bool val_readed = true;
    int src_v;
    src>>src_v;

    while (i < n && val_readed) {
        if (arr[i] > src_v) {
            dst << src_v<<" ";
            if (!src.eof())
                src >> src_v;
            else
                val_readed = false;
        }
        else {
            dst << arr[i++]<<" ";
        }
    }

    while (i < n) {
        dst << arr[i++];
        if (i < n)dst << " ";
    }

    if (val_readed) {
        do {
            dst << src_v;
            if (!src.eof())
                src >> src_v;
            else
                val_readed = false;
            if (val_readed) dst << " ";            
        } while (val_readed);
    }


    src.close();
    dst.close();
    return rf;
}

void PrintArr(string pref,int *arr,int n) {
    cout << pref << " = ";
    for (int i = 0; i < n; i++)
        cout << arr[i]<<" ";
    cout << endl;
}
void PrintFile(string pref, string fn) {
    cout << pref << " = ";
    fstream f(fn, ios::in);
    if (!f.is_open()) return;
    int v;
    while (!f.eof()) {
        f >> v;
        cout << v << " ";
    }
    cout << endl;
    f.close();
}

void TestFile(string fn) {    
    fstream f(fn, ios::in);
    if (!f.is_open()) {
        cout << "file not opend."<< endl;
        return;
    }
    int v;  
    int lv = -1;
    while (!f.eof()) {
        f >> v;
        if (lv > v) {
            cout << "Error!"<<endl;
            return;
        }       
        lv = v;
    }
    cout << "sucess!" << endl;
    f.close();
}



void DoItForFile(string fn) {
    remove("step0.txt");
    remove("step1.txt");

    fstream f(fn, ios::in);
    if (!f.is_open()) return;
    int cnt = 0;
    int inArr[NREAD];
    string resfile;
    do {
        cnt = ReadPart(f, inArr);
        
        if (cnt > 0) {
            resfile = Merge(inArr, cnt);
            //PrintArr("part", inArr, cnt);
            //PrintFile("step:", resfile);
        }
    } while (cnt>0);
    f.close();

    if (resfile.length() > 0) {
        //PrintFile("   src:", fn);
        //PrintFile("result:", resfile);
        TestFile(resfile);        
    }
    else {
        cout << "oops"<< endl;
    }

}
int main()
{
    generate();
    DoItForFile(SFP);    
}
