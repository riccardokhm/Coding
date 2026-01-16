//Project to create an online help system for control instructions in cpp.

#include <iostream>
using namespace std;

int main(){
    for (;;){

        int choice;
        do{
            cout << "Welcome to the C++ Control Instructions Help System!" << endl;
            cout << "Please select a topic to learn more about:" << endl;
            cout << "1. If Statements" << endl;
            cout << "2. Switch Statements" << endl;
            cout << "3. For Loops" << endl;
            cout << "4. While Loops" << endl;
            cout << "5. Do-While Loops" << endl;
            cout << "6. Breaak" << endl;
            cout << "7. Continue" << endl;
            cout << "8. Goto" << endl;
            cout << "q. Exit" << endl;

            cin >> choice;
        }while(choice < 0 || choice > 5 != 'q');
        
        if (choice == 'q') break;

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
            case 6:
                cout << "Break Statement:" << endl;
                cout << "   Syntax: break;" << endl;
                cout << "   Used to exit loops or switch statements prematurely." << endl;
                break;
            case 7:
                cout << "Continue Statement:" << endl;
                cout << "   Syntax: continue;" << endl;
                cout << "   Used to skip the rest of the loop body and continue with the next iteration." << endl;
                break;
            case 8:
                cout << "Goto Statement:" << endl;
                cout << "   Syntax: goto label;" << endl;
                cout << "   Used to jump to a labeled statement in the same function." << endl;
                break;
        }
    }

    system("pause");
}