#include "QSort_ST.h"

static void quicksort( std::vector<int> &arr, long left, long right) {
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
    quicksort(arr, left, right_bound);
    quicksort(arr, left_bound, right);
}
void quicksort_st(std::vector<int>& arr) {
    quicksort(arr,0,arr.size()-1);
}