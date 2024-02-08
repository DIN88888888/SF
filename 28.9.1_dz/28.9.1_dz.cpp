#include <iostream>
#include <chrono>
#include <string>
#include <future>
#include <mutex>

using std::cout; 
using std::cin; 
using std::string;


bool Thr = false;
int thrC = 0;
int max_Thr = 10;
std::mutex mut;

void Merge(int* arr, int b, int m, int e, int* ab)
{
    int i = b;
    int j = m;

    for (int k = b; k < e; k++) {
        if (i < m && (j >= e || ab[i] <= ab[j])) {
            arr[k] = ab[i];
            i = i + 1;
        }
        else {
            arr[k] = ab[j];
            j = j + 1;
        }
    }
}

void SplitMerge(int* ab, int b, int e, int* arr)
{
    if (e - b <= 1)
        return;
    int m = (e + b) / 2;
    if (Thr && thrC < max_Thr) {
        mut.lock();
        thrC++;
        mut.unlock();

        std::future<void> f = std::async(std::launch::async, [&]() { SplitMerge(arr, b, m, ab); });
        SplitMerge(arr, m, e, ab);
        f.get();
        mut.lock();
        thrC--;
        mut.unlock();
    }
    else {
        SplitMerge(arr, b, m, ab);
        SplitMerge(arr, m, e, ab);
    }    

    Merge(ab, b, m, e, arr);
}

void MergeSort(int* arr, int n)
{
    int* arrB = new int[n];
    memcpy(arrB, arr, sizeof(int) * n);
    SplitMerge(arr, 0, n, arrB);
    delete[] arrB;
}



bool testArr(int* arr, int n) {
    for (int i = 0; i < n-1; i++) {
        if (arr[i] > arr[i + 1]) return false;
    }
    return true;
}

void printArr(int* arr, int n, string txt, string txt2) {
    if (n > 20)
    {
        cout << txt2;
        return;
    }
    cout << txt;
    
    for (int i = 0; i < n;i++) {
        cout << arr[i] << ' ';
    }
    cout << '\n';
}

int main()
{
    srand(std::chrono::system_clock::now().time_since_epoch().count());
    bool doit = true;
    int arrsize = 0;
    while (doit) {
        //system("clrscr");
        std::cout << "Enter array size (enter <=20 to see numbers or <=0 to exit):";
        cin >> arrsize;
        doit = arrsize > 0;

        //work
        if (doit) {
            //create and fill arrays
            int *arr1 = new int[arrsize];
            int *arr2 = new int[arrsize];
            for (int i = 0; i < arrsize; i++) {
                int v = rand() % 100;
                arr1[i] = v;
                arr2[i] = v;
            }               
            printArr(arr1,arrsize, "Before sort: ","");

            //sort

            time_t start, end;

            Thr = false;
            time(&start);
            MergeSort(arr1,arrsize);
            time(&end);
            double ST = difftime(end, start);


            Thr = true;
            thrC = 0;
            time(&start);
            MergeSort(arr2, arrsize);
            time(&end);
            double MT = difftime(end, start);

            //test and print result
            if (testArr(arr1, arrsize)) {
                printArr(arr1, arrsize, string("After sort ST(") + std::to_string(ST) + "s): ", string("Single thread OK ") + std::to_string(ST) + "s\n");
            }
            else
            {
                cout << "Single Thread arr bad!\n";
            }

            if (testArr(arr2, arrsize)) {
                printArr(arr2, arrsize, string("After sort MT(") + std::to_string(MT) + "s): ", string("Multi thread OK ") + std::to_string(MT) + "s\n");
            }
            else
            {
                cout << "Multi Thread arr bad!\n";
            }
            
            // free mem
            delete[] arr1;
            delete[] arr2;
        }
        cout << "\n\n\n";
    }
    return 0;
}
