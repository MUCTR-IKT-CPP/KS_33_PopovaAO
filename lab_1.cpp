#include <iostream>

using namespace std;

/*
 * Проверка массива на возрастание
 * 
 * @param arr - массив целых чисел
 * @param n - количество элементов в массиве
 * @return true, если последовательность строго возрастающая, иначе false
 */
bool isIncreasing(int* arr, int n) {
    int count = 0;
    
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] < arr[i + 1]) {
            count++;
        }
    }
    
    return count == n - 1;
}

/*
 * Проверка массива на убывание
 * 
 * @param arr - массив целых чисел
 * @param n - количество элементов в массиве
 * @return true, если последовательность строго убывающая, иначе false
 */
bool isDecreasing(int* arr, int n) {
    int count = 0;
    
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            count++;
        }
    }
    
    return count == n - 1;
}

int main() {
    const int MAX_SIZE = 100;
    int arr[MAX_SIZE];
    int n;
    
    cout << "Enter array size: ";
    cin >> n;
    
    cout << "Enter " << n << " numbers: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    if (n == 1) {
        cout << "Neither" << endl;
    }
    else if (isIncreasing(arr, n)) {
        cout << "Increasing" << endl;
    } 
    else if (isDecreasing(arr, n)) {
        cout << "Decreasing" << endl;
    } 
    else {
        cout << "Neither" << endl;
    }
    
    return 0;
}