//Project to create an online help system for control instructions in cpp.

#include <iostream>
using namespace std;

class Help{
    int id;

    public:
    void helpon(char topic);
    void showmenu();
    bool isvalid(char ch);
    int get_id(){
        return id;
    }

    Help(){
        id = 0;
    }
};

void Help::helpon(char topic){
    switch(topic){
        case '1':
            cout << "If Statements:" << endl;
            cout << "   Syntax: if (condition) { // code to execute }" << endl;
            cout << "   (Additional) else if/else { // code to execute }" << endl;
            break;

        case '2':
            cout << "Switch Statements:" << endl;
            cout << "   Syntax: switch (variable) { case value1: // code; break; case value2: // code; break; default: // code; }" << endl;
            break;
        case '3':
            cout << "For Loops:" << endl;
            cout << "   Syntax: for (initialization; condition; increment) { // code to execute }" << endl;
            break;

        case '4':
            cout << "While Loops:" << endl;
            cout << "   Syntax: while (condition) { // code to execute }" << endl;
            break;

        case '5':
            cout << "Do-While Loops:" << endl;
            cout << "   Syntax: do { // code to execute } while (condition);" << endl;
            break;
    }
}

void Help::showmenu(){
    cout << "Welcome to the C++ Control Instructions Help System!" << endl;
    cout << "Please select a topic to learn more about:" << endl;
    cout << "1. If Statements" << endl;
    cout << "2. Switch Statements" << endl;
    cout << "3. For Loops" << endl;
    cout << "4. While Loops" << endl;
    cout << "5. Do-While Loops" << endl;
    cout << "q. Exit" << endl;
}

bool Help::isvalid(char ch){
    if (ch < '1' || ch > '5' && ch != 'q'){
        cout << "Invalid selection. Please try again." << endl;
        return false;
    }
    return true;
}


int main(){
    for (;;){

        int choice;

        Help helpobj;
        helpobj.showmenu();

        char answer;
        cin >> answer;
        if (answer == 'q') break;
        if (helpobj.isvalid(answer)) {
            helpobj.helpon(answer);
        }
    }

    system("pause");
    return 0;
}