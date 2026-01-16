#include <cstdlib>
#include <iostream>
using namespace std;

int main(){

    int number, sum = 0;

    cout<< "Enter the number up to which you want to sum: ";
    cin>> number;

    for(int i = 1; i <= number; sum +=i ++);

    cout << "The sum of numbers from 1 to " << number << " is: " << sum << endl;

    system("pause");
}
