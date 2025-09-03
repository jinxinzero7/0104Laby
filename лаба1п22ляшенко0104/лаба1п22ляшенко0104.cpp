// лаба1п22ляшенко0104.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <algorithm>

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
};