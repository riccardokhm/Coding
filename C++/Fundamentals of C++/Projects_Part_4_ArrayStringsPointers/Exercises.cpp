// Various exercises on array and pointers in C++
#include <iostream>
#include <cstring>

using namespace std;

int main(){


    cout << "Choose the exercise to run (1-3): " << endl;
    cout << "1. Find duplicated value in an array" << endl;
    cout << "2. Compare strings without keeping in consideration the case" << endl;
    cout << "3. Count the number of upper case letters in a string" << endl;
    cout << "4. Exit" << endl;
    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        findDuplicateInArray();
        break;
    case 2:
        compareStringsIgnoreCase();
        break;
    case 3:
        countUpperCaseLetters();
        break;
    case 4:
        cout << "Exiting program." << endl;
        return 0;
    default:
        cout << "Invalid choice. Please select a valid option." << endl;
    }
}


void findDuplicateInArray() {
    const int SIZE = 10;
    cout << "Finding duplicate in an array of size " << SIZE << endl;
    cout << "Please provide ten integers:" << endl;
    int arr[SIZE];
    for(int i = 0; i < SIZE; i++) {
        cin >> arr[i];
    }

    bool foundDuplicate = false;

    for(int i = 0; i < SIZE; i++) {
        for(int j = i + 1; j < SIZE; j++) {
            if(arr[i] == arr[j]) {
                cout << "Duplicate value found in position " << i << ": " << arr[i] << endl;
                foundDuplicate = true;
            }
        }

    }

    if(!foundDuplicate) {
        cout << "No duplicate values found." << endl;
    }
}


void compareStringsIgnoreCase() {
    const int MAX_LENGTH = 100;
    char str1[MAX_LENGTH];
    char str2[MAX_LENGTH];

    cout << "Enter first string (max 100 char): ";
    cin.ignore(); // To clear the newline character from the input buffer
    cin.getline(str1, MAX_LENGTH);
    cout << "Enter second string (max 100 char): ";
    cin.getline(str2, MAX_LENGTH);

    // Convert both strings to lowercase for comparison
    for(int i = 0; str1[i]; i++) {
        str1[i] = tolower(str1[i]);
    }
    for(int i = 0; str2[i]; i++) {
        str2[i] = tolower(str2[i]);
    }

    if(strcmp(str1, str2) == 0) {
        cout << "The strings are equal (case insensitive)." << endl;
    } else {
        cout << "The strings are not equal." << endl;
    }
}


void countUpperCaseLetters(){
    const int MAX_LENGHT = 100;
    char str[MAX_LENGHT];
    int count = 0;
    cout << "Enter a string (max 100 char): ";

    for (int i = 0; i < MAX_LENGHT; i++) {
        if (isupper(str[i])) {
            count++;
        }
    }

    cout << "Number of upper case letters: " << count << endl;
    system("pause");
    return;

}
