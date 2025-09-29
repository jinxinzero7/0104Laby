#include <iostream>
#include <algorithm>
#include <Windows.h>

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

void demo_malloc_free()
{
    cout << "malloc/free:" << endl;
    cout << "Enter the array size: ";
    int arr_size;
    cin >> arr_size;

    int* arr = (int*)malloc(arr_size * sizeof(int));
    if (arr == NULL) {
        cout << "Memory allocation failed" << endl;
        return;
    }

    for (int i = 0; i < arr_size; i++)
    {
        arr[i] = fib(i + 1);
    }

    cout << "Original array: ";
    for (int i = 0; i < arr_size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    reverse(arr, arr + arr_size);

    cout << "Reversed array: ";
    for (int i = 0; i < arr_size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    free(arr);
}

void demo_new_delete()
{
    cout << "new/delete:" << endl;
    cout << "Enter the array size: ";
    int arr_size;
    cin >> arr_size;

    int* arr = new int[arr_size];

    for (int i = 0; i < arr_size; i++)
    {
        arr[i] = fib(i + 1);
    }

    cout << "Original array: ";
    for (int i = 0; i < arr_size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    reverse(arr, arr + arr_size);

    cout << "Reversed array: ";
    for (int i = 0; i < arr_size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    delete[] arr;
}

void demo_virtual_alloc()
{
    cout << "VirtualAlloc/VirtualFree:" << endl;
    cout << "Enter the array size: ";
    int arr_size;
    cin >> arr_size;

    LPVOID mem = VirtualAlloc(NULL, arr_size * sizeof(int), MEM_COMMIT, PAGE_READWRITE);
    if (mem == NULL) {
        cout << "VirtualAlloc failed" << endl;
        return;
    }

    int* arr = (int*)mem;

    for (int i = 0; i < arr_size; i++)
    {
        arr[i] = fib(i + 1);
    }

    cout << "Original array: ";
    for (int i = 0; i < arr_size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    reverse(arr, arr + arr_size);

    cout << "Reversed array: ";
    for (int i = 0; i < arr_size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    VirtualFree(mem, 0, MEM_RELEASE);
}

void demo_heap_alloc()
{
    cout << "HeapAlloc/HeapFree:" << endl;
    cout << "Enter the array size: ";
    int arr_size;
    cin >> arr_size;

    HANDLE hHeap = GetProcessHeap();
    int* arr = (int*)HeapAlloc(hHeap, 0, arr_size * sizeof(int));
    if (arr == NULL) {
        cout << "HeapAlloc failed" << endl;
        return;
    }

    for (int i = 0; i < arr_size; i++)
    {
        arr[i] = fib(i + 1);
    }

    cout << "Original array: ";
    for (int i = 0; i < arr_size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    reverse(arr, arr + arr_size);

    cout << "Reversed array: ";
    for (int i = 0; i < arr_size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    HeapFree(hHeap, 0, arr);
}

void demo_custom_heap()
{
    cout << "Custom Heap:" << endl;

    HANDLE hHeap = HeapCreate(0, 0, 0);
    if (hHeap == NULL) {
        cout << "Heap creating failed";
        return;
    }

    size_t arr_size;
    cout << "Enter the array size: ";
    cin >> arr_size;

    int** arr = (int**)(HeapAlloc(hHeap, HEAP_ZERO_MEMORY, arr_size * sizeof(int*)));
    if (!arr) {
        HeapDestroy(hHeap);
        return;
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
            return;
        }
        *arr[i] = fib(i + 1);
    }

    cout << "Original array: ";
    for (size_t i = 0; i < arr_size; ++i) {
        cout << *arr[i] << " ";
    }
    cout << endl;

    reverse(arr, arr + arr_size);

    cout << "Reversed array: ";
    for (size_t i = 0; i < arr_size; ++i) {
        cout << *arr[i] << " ";
    }
    cout << endl;

    for (size_t i = 0; i < arr_size; ++i) {
        HeapFree(hHeap, 0, arr[i]);
    }
    HeapFree(hHeap, 0, arr);
    HeapDestroy(hHeap);
}

int main()
{
    SetConsoleOutputCP(1251);

    demo_malloc_free();
    cout << endl;

    demo_new_delete();
    cout << endl;

    demo_virtual_alloc();
    cout << endl;

    demo_heap_alloc();
    cout << endl;

    demo_custom_heap();

    return 0;
}
