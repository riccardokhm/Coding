// Program to convert a given character from uppercase to lowercase and vice versa, counting the numer of conversions.
#include <iostream>
using namespace std;

int main(){
    
    int conversionCount = 0;
    char ch;

    for (; ; conversionCount++)
    {
        cout << "Enter a character (press . to stop): ";
        cin >> ch;

        if (ch == '.'){
            cout << "Total conversions made: " << conversionCount << endl;
            break;
        }
        else if (ch >= 'A' && ch <= 'Z')
            cout << "Converted to lowercase: " << char(ch + 32) << endl;
        else if (ch >= 'a' && ch <= 'z')
            cout << "Converted to uppercase: " << char(ch - 32) << endl;
        else
            cout << "Character is not an alphabetic letter. Please provide a valid value" << endl;
    }
    
}