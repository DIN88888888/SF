#include "QSort_TP.h"
#include "ThreadPool.h"

static void quicksort(std::vector<int>& arr, long left, long right) {
    if (left >= right) return;
    long left_bound = left;
    long right_bound = right;

    long middle = arr[(left_bound + right_bound) / 2];

    do {
        while (arr[left_bound] < middle) {
            left_bound++;
        }
        while (arr[right_bound] > middle) {
            right_bound--;
        }

        //Меняем элементы местами
        if (left_bound <= right_bound) {
            std::swap(arr[left_bound], arr[right_bound]);
            left_bound++;
            right_bound--;
        }
    } while (left_bound <= right_bound);
    
    if (right_bound - left > 10000)
    {
        TP::job->push_task([=, &arr]() {quicksort(arr, left, right_bound); });
        TP::job->push_task([=, &arr]() {quicksort(arr, left_bound, right); });        
    }
    else {
        quicksort(arr, left, right_bound);
        quicksort(arr, left_bound, right);
    }
    //std::cout << "quicksort tp ends\n";
}
void quicksort_tp(std::vector<int> &arr) {
    jobType j = TP::pool->make_job();
    j.get()->Set_Start_Function([&arr]() {quicksort(arr, 0, arr.size() - 1); });
    j.get()->Start();
    j.get()->Wait();
}