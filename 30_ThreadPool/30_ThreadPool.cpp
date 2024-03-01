#include <iostream>
#include <chrono>
#include "ThreadPool.h"
#include "QSort_MT.h"
#include "QSort_ST.h"
#include "QSort_TP.h"
#include <thread>
#include <string>

#define VSIZE 80000000

using namespace std;
bool arrtest(vector<int> &arr) {
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i - 1] > arr[i])
            return false;
    }
    return true;
}
void printarr(vector<int> &arr) {
    cout << "data = [";
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i]<<",";
    }
    cout << "]\n";
}

int main()
{
    {
        srand(time(0));
        //base data for tests
        vector<int> arr;
        for (int i = 0; i < VSIZE; i++) {
            arr.push_back(rand() % 5000);
        }
        //timers
        clock_t begin_time, end_time;
        //copy for test instance
        vector<int> tmp,tmp2;
        std::cout << "Base array size = " << arr.size() << " test = " << ((arrtest(arr)) ? "OK" : "BAD") << '\n';
        if (arr.size() < 50) printarr(arr);



        //single thread
        if (arr.size() <= 80000000) {
            tmp = arr;
            begin_time = clock();
            quicksort_st(tmp);
            end_time = clock();
            std::cout << "Single TH Quick Sort takes " << (0.0f + end_time - begin_time) / CLOCKS_PER_SEC << " sec   test " << ((arrtest(tmp)) ? "OK" : "BAD") << '\n';
            if (tmp.size() < 50) printarr(tmp);
        }
        else {
            cout << "array too big for single core test.\n";//>20 sec
        }



        //regular multi thread
        tmp = arr;
        begin_time = clock();
        quicksort_mt(tmp);
        end_time = clock();
        std::cout << "Multi TH Quick Sort takes " << (0.0f + end_time - begin_time) / CLOCKS_PER_SEC << " sec   test " << ((arrtest(tmp)) ? "OK" : "BAD") << '\n';
        if (tmp.size() < 50) printarr(tmp);

        //poooled q sort
        TP thPool;
        tmp = arr;
        begin_time = clock();        
        quicksort_tp(tmp);
        end_time = clock();
        std::cout << "Pool Quick Sort takes " << (0.0f + end_time - begin_time) / CLOCKS_PER_SEC << " sec   test " << ((arrtest(tmp)) ? "OK" : "BAD") << '\n';
        if (tmp.size() < 50) printarr(tmp);
    }
    char fff;
    cout << "Press F>>";
    cin >> fff;
    
}