//Project to create an online help system for control instructions in cpp.

#include <iostream>
using namespace std;

int main(){

    int choice;
    do{
        cout << "Welcome to the C++ Control Instructions Help System!" << endl;
        cout << "Please select a topic to learn more about:" << endl;
        cout << "1. If Statements" << endl;
        cout << "2. Switch Statements" << endl;
        cout << "3. For Loops" << endl;
        cout << "4. While Loops" << endl;
        cout << "5. Do-While Loops" << endl;
        cout << "0. Exit" << endl;

        cin >> choice;
    }while(choice < 0 || choice > 5);


    switch (choice)
    {
        case 0:
            cout << "Exiting the Help System. Goodbye!" << endl;
            break;
        case 1:
            cout << "If Statements:" << endl;
            cout << "   Syntax: if (condition) { // code to execute }" << endl;
            cout << "   (Additional) else if/else { // code to execute }" << endl;
            break;

        case 2:
            cout << "Switch Statements:" << endl;
            cout << "   Syntax: switch (variable) { case value1: // code; break; case value2: // code; break; default: // code; }" << endl;
            break;
        case 3:
            cout << "For Loops:" << endl;
            cout << "   Syntax: for (initialization; condition; increment) { // code to execute }" << endl;
            break;

        case 4:
            cout << "While Loops:" << endl;
            cout << "   Syntax: while (condition) { // code to execute }" << endl;
            break;

        case 5:
            cout << "Do-While Loops:" << endl;
            cout << "   Syntax: do { // code to execute } while (condition);" << endl;
            break;
    }

    system("pause");
}