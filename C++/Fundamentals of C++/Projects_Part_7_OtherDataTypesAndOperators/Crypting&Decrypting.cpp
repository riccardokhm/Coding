// Using the XOR operator to encrypt and decrypt a message
#include <iostream>

using namespace std;

int main(){
    char message[] = "This is a test";
    char key = 88;

    cout << "Original message: " << message << endl;

    for (int i = 0; message[i] != '\0'; i++){
        message[i] = message[i] ^ key;
    }

    cout << "Encrypted message: " << message << endl;

    for (int i = 0; message[i] != '\0'; i++){
        message[i] = message[i] ^ key;
    }

    cout << "Decrypted message: " << message << endl;

    system("pause");
    return 0;
}
