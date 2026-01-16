//Project to create an online help system for control instructions in cpp.

#include <iostream>
using namespace std;

int main(){
    cout << "Welcome to the C++ Control Instructions Help System!" << endl;
    cout << "Please select a topic to learn more about:" << endl;
    cout << "1. If Statements" << endl;
    cout << "2. Switch Statements" << endl;

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        cout << "If Statements:" << endl;
        cout << "   Syntax: if (condition) { // code to execute }" << endl;
        cout << "   (Additional) else if/else { // code to execute }" << endl;
        break;

    case 2:
        cout << "Switch Statements:" << endl;
        cout << "   Syntax: switch (variable) { case value1: // code; break; case value2: // code; break; default: // code; }" << endl;
        break;
    
    default:
        cout << "Invalid choice. Please select a valid topic." << endl;
        break;
    }

    system("pause");
}