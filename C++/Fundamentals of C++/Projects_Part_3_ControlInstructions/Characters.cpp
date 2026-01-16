#include <iostream>
using namespace std;

int main(){
    unsigned char ch;

    ch = 32; // ASCII value for space character
    while(ch){
        cout << ch; // Print the character corresponding to the ASCII value
        ch++; // Move to the next ASCII value
    }

    cout << endl;

    system ("pause");
    return 0;
}