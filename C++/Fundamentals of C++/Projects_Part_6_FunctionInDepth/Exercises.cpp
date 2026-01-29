// Exercises on Module 6: Functions in Depth

#include <iostream>
#include <cmath>

using namespace std;

/// @brief Rounds a given double number to the nearest integer and prints the result.
/// @param number 
void roundMe(double &number) {
    double intPart;

    double fracPart = modf(number, &intPart);
    if (fracPart > 0.5)
    {
        intPart += 1.0;
    }
    else if (fracPart < -0.5)
    {
        intPart -= 1.0;
    }
    
    cout << "Rounding " <<  intPart << " gives " << std::round(number) << endl;
}

/// @brief Swaps the values of two integers using references and return the minimum of the two values.
int min_swap(int &a, int &b) {

    int temp;

    temp = a;
    a = b;
    b = temp;
    cout << "After swapping, a = " << a << ", b = " << b << endl;
    if (a>b)
    {
        return b;
    }
    else
    {
        return a;
    }

}


/// @brief Print an integer with optional tab indentation.
/// @param i 
/// @param tabsNumber 
void println(int i, int tabsNumber = 0){
    if (!tabsNumber)
    {
        cout << i << endl;
    }
    else
    {
        for (int j = 0; j < tabsNumber; j++)
        {
            cout << "\t";
        }
        cout << i << endl;
    }

}