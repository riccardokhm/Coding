// Arithmetics with pointers 
#include <iostream>
using namespace std;

int main(){

    int* i, j[10];
    double* f, g[10];

    int x;

    i = j; // pointer to int assigned to an array of int
    f = g; // pointer to double assigned to an array of double

    for (x = 0; x < 10; x++)
    {
        cout << i + x << " "; // address of j[x]
        cout << f + x << " "; // address of g[x]
        cout << endl;
    }
    
    system("pause");
    return 0;

}