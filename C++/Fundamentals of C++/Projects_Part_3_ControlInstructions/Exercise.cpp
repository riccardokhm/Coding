// A program allowing the user to type and visualize on screen the text until he/she decides to stop (by pressin $): the program 
// will count the number of characters typed (excluding the $ character) and will display it on screen.

#include <iostream>
#include <cstdlib>

using namespace std;

int main(){

    bool isTyping = true;
    char character;
    int charCount = 0;

    // Prompting user to start typing
    do  
    {
        cout << "Start typing (press $ to stop): " << endl;
        cin >> character;

        if (character == '$')
        {
            isTyping = false;
            break;
        }
        else
            charCount++;
    }
    while (isTyping);

    cout << "Number of characters typed: " << charCount << endl;
    

    system("PAUSE");
    return 0;
}