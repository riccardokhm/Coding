// Simple program with basic pointer operations
#include <iostream>

using namespace std;

int main(){

    int var;

    cout << "Pointer Basics in C++" << endl;
    cout << "Provide a number for demonstration:" << endl;
    cin >>var;
  
    int *ptr = &var;

    cout << "Value of var: " << var << endl;
    cout << "Address of var: " << &var << endl;
    cout << "Value of ptr (address of var): " << ptr << endl;
    cout << "Value pointed to by ptr: " << *ptr << endl;

    (*ptr)++; // Increment the value at the address stored in ptr
    cout << "New value of var after incrementing via ptr: " << var << endl;

    system("pause");
    return 0;
}