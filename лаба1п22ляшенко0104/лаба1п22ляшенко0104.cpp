#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <stdio.h>


using namespace std;

int fib(int n)
{
    if (n <= 0) {
        return 0;
    }
    int a = 0, b = 1;
    for (int i = 2; i <= n; i++)
    {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main()
{
    // задание 1
    /*
    cout << "Enter the array size" << endl;
    int arr_size;
    cin >> arr_size;
    cout << endl;

    int* arr = new int[arr_size];
    for (int i = 0; i < arr_size; i++)
    {
        arr[i] = fib(i+1);
    }

    std::reverse(arr, arr + arr_size);

    for (int i = 0; i < arr_size; i++) {
        cout << arr[i] << " ";
    }

    delete[] arr; // Освобождаем выделенную память
    return 0;
    */

    //задание2

    HANDLE hHeap = HeapCreate(0, 0, 0);

    if (hHeap == NULL) {
        cout << "Heap creating failed";
        return 1;
    }

    size_t arr_size;
    cout << "Enter the array size: ";
    cin >> arr_size;

    int** arr = (int**)(HeapAlloc(hHeap, HEAP_ZERO_MEMORY, arr_size * sizeof(int*)));
    if (!arr) {
        HeapDestroy(hHeap);
        return 1;
    }

    for (size_t i = 0; i < arr_size; ++i) {
        arr[i] = (int*)(HeapAlloc(hHeap, 0, sizeof(int)));
        if (!arr[i]) {
            cout << "Heap memory alloc is failed";
            for (size_t j = 0; j < i; ++j) {
                HeapFree(hHeap, 0, arr[j]);
            }
            HeapFree(hHeap, 0, arr);
            HeapDestroy(hHeap);
            return 1;
        }
        *arr[i] = fib(i+1);
    }

    reverse(arr, arr + arr_size);

    cout << "Array created:" << endl;
    for (size_t i = 0; i < arr_size; ++i) {
        cout << *arr[i] << " ";
    }
    cout << endl;

    for (size_t i = 0; i < arr_size; ++i) {
        HeapFree(hHeap, 0, arr[i]);
    }
    HeapFree(hHeap, 0, arr);

    HeapDestroy(hHeap);

    return 0;
};
