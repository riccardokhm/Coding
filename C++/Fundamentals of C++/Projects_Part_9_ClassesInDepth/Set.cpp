// Creating a set class to demonstrate basic class functionalities in C++
#include <iostream>
using namespace std;

const int MaxSize = 100;

class Set {
    int len;
    char member[MaxSize];

    int find(char ch);

public:
    Set(){len = 0;} // Constructor to initialize length

    int getLength() { return len; } // Method to get current length of the set
    void showset(); // Method to display the set members
    bool isMember(char ch); // Method to check if a character is a member of the set

    Set operator+(char ch); // Overloaded + operator to add a character to the set
    Set operator-(char ch); // Overloaded - operator to remove a character from the set
    Set operator+(Set& s); // Overloaded + operator to perform union of two sets
    Set operator-(Set& s); // Overloaded - operator to perform difference of two sets
    bool operator>(Set& s); // Overloaded > operator to check if current set is a superset of another set
    bool operator<(Set& s); // Overloaded < operator to check if current set is a subset of another set
    Set operator&(Set& s); // Overloaded & operator to perform intersection of two sets
    Set operator|(Set& s); // Overloaded | operator to perform symmetric difference of two sets
};

/// @brief Return the index of character ch in the set, or -1 if not found
/// @param ch 
/// @return 
int Set::find(char ch) {
    for (int i = 0; i < len; i++) {
        if (member[i] == ch) {
            return i;
        }
    }
    return -1;
}

void Set::showset() {
    for (int i = 0; i < len; i++) {
        cout << member[i] << " ";
    }
    cout << endl;
}

bool Set::isMember(char ch) {
    return find(ch) != -1;
}

Set Set::operator+(char ch) {
    Set temp = *this;
    if (temp.find(ch) == -1 && temp.len < MaxSize) {
        temp.member[temp.len++] = ch;
    }
    return temp;
}

Set Set::operator-(char ch) {
    Set temp = *this;
    int index = temp.find(ch);
    if (index != -1) {
        for (int i = index; i < temp.len - 1; i++) {
            temp.member[i] = temp.member[i + 1];
        }
        temp.len--;
    }
    return temp;
}

Set Set::operator+(Set& s) {
    Set temp = *this;
    for (int i = 0; i < s.len; i++) {
        temp = temp + s.member[i];
    }
    return temp;
}

Set Set::operator-(Set& s) {
    Set temp = *this;
    for (int i = 0; i < s.len; i++) {
        temp = temp - s.member[i];
    }
    return temp;
}

bool Set::operator>(Set& s){
    Set temp = *this;
    if (temp.len <= s.len) {
        return false;
    }
    for (int i = 0; i < s.len; i++) {
        if (!temp.isMember(s.member[i])) {
            return false;
        }
    }
    return true;
}

bool Set::operator<(Set& s){
    Set temp = *this;
    if (temp.len >= s.len) {
        return false;
    }
    for (int i = 0; i < temp.len; i++) {
        if (!s.isMember(temp.member[i])) {
            return false;
        }
    }
    return true;
}

Set Set::operator&(Set& s){
    Set temp = *this;
    Set result;
    for (int i = 0; i < temp.len; i++) {
        if (s.isMember(temp.member[i])) {
            result = result + temp.member[i];
        }
    }
    return result;
}

Set Set::operator|(Set& s){
    Set temp= *this, result;
    for (int i = 0; i < temp.len; i++) {
        if (!s.isMember(temp.member[i])) {
            result = result + temp.member[i];
        }
    }
    for (int i = 0; i < s.len; i++) {
        if (!temp.isMember(s.member[i])) {
            result = result + s.member[i];
        }
    }
    return result;
}