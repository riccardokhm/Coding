// Generate overload functions for different data types to demonstrate function overloading in C++.

#include <iostream>
using namespace std;

// Print function prototypes
void print(int i);
void print(double f);
void print(char ch);
void print(const char* str);
void print(bool b);
void print(long l);

// Println function definitions
void println(int i);
void println(double f);
void println(char ch);
void println(const char* str);
void println(bool b);
void println(long l);

#pragma region Println function definitions

void println(int i) {
    cout << i << endl;
}

void println(double f) {
    cout << f << endl;
}

void println(char ch) {
    cout << ch << endl;
}

void println(const char* str) {
    cout << str << endl;
}

void println(bool b) {
    cout << (b ? "true" : "false") << endl;
}

void println(long l) {
    cout << l << endl;
}

#pragma endregion

#pragma region Print function definitions

void print(int i) {
    cout << i;
}

void print(double f) {
    cout << f;
}

void print(char ch) {
    cout << ch;
}

void print(const char* str) {
    cout << str;
}

void print(bool b) {
    cout << (b ? "true" : "false");
}

void print(long l) {
    cout << l;
}

#pragma endregion

int main() {
    print(42);
    cout << " ";
    print(3.14);
    cout << " ";
    print('A');
    cout << " ";
    print("Hello, World!");
    cout << " ";
    print(true);
    cout << " ";
    print(1234567890L);
    cout << endl;

    println(42);
    println(3.14);
    println('A');
    println("Hello, World!");
    println(true);
    println(1234567890L);

    system("pause");    
    return 0;
}