// Project on the use of XOR operation in C++26

#include <iostream>
#include <cmath>

using namespace std;

int main()
{
    bool p, q;

    cout << boolalpha;

    cout << "Enter two boolean values (0 or 1) for p and q: ";
    cout << "p = " << endl;
    cin >> p;
    cout << "q = " << endl;
    cin >> q;

    bool result = p ^ q; // XOR operation, alternatively using p ^ q

    cout << "The result of p XOR q is: " << result << endl;
}