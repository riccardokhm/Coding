// Inverting a string using pointers
#include <iostream>
#include <cstring>

using namespace std;

int main(){
    char str[] = "This is a test string";
    char *start, *end;
    int len;
    char t;

    cout << "Original string: " << str << endl;
    len = strlen(str);
    start = str;
    end = &str[len - 1];

    while (start < end){
        t = *start;
        *start = *end;
        *end = t;
        start++;
        end--;
    }

    cout << "Reversed string: " << str << endl;
    system("pause");
    return 0;
}