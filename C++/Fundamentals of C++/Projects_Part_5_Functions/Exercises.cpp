// Various function exercises in C++

#include <iostream>
#include <cmath>
using namespace std;

float computeHypotenuse(float a, float b) {
    return sqrt(a * a + b * b);
}

float myStrLeng(char *str){
    float length = 0;
    while (*str) {
        length++;
        str++;
    }
    return length;
}

/// @brief Returns an array starting from @n with the 4 successive integers whose reciprocal distance is three.
/// @param n 
/// @return 
int* byThrees(int n) {
    int* arr = new int[5];
    arr[0] = n;
    for (int i = 1; i <= 4; i++) {
        arr[i] = arr[i - 1] + 3;
    }
    return arr;
}

/// @brief Prints numbers from 1 to n in descending order.
/// @param n 
void printNumbers(int n){
    if (n == 1) {
        cout << n << " ";
        return;
    }   
    cout << n << " ";
    printNumbers(n - 1);
}