// Bubble sorting algorithm in C++
#include <iostream>
using namespace std;

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap arr[j] and arr[j+1]
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(){
    int arr[5];
    cout << "Enter 5 integers to sort: ";
    for(int i = 0; i < 5; i++){
        cin >> arr[i];
    }
    cout << "Unsorted array: ";
    for(int i = 0; i < 5; i++){
        cout << arr[i] << " ";
    }

    bubbleSort(arr, 5);
    cout << "Sorted array: ";
    for(int i = 0; i < 5; i++){
        cout << arr[i] << " ";
    }
    system("pause");
    return 0;
}