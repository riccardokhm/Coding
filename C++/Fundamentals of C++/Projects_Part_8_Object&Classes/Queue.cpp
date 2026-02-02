// Generate a class for a non-circular Queue data structure 
#include <iostream>
using namespace std;

const int maxQsize = 100; // maximum size of the Queue

class Queue {
private:
    char items[maxQsize];
    int size; // actual size of the Queue
    int putloc, getloc; // put and get indices
public:
    Queue(int len) {
        if(len > maxQsize) {
            cout << "Size exceeds maximum. Setting to maxQsize." << endl;
            size = maxQsize;
        } else if (len <= 0) {
            size = 1;
        } else {
            size = len;
        }
        putloc = getloc = 0;
    }

    void put(char ch) {
        if (putloc == size) {
            cout << " - Queue is full." << endl;
            return;
        }
        items[putloc++] = ch;
    }

    char get() {
        if (getloc == putloc) {
            cout << " - Queue is empty." << endl;
            return '\0';
        }
        return items[getloc++];
    }
}; 

class QueueCircular {
private:
    char items[maxQsize];
    int size; // actual size of the Queue
    int putloc, getloc; // put and get indices
public:
    QueueCircular(int len) {
        if(len > maxQsize) {
            cout << "Size exceeds maximum. Setting to maxQsize." << endl;
            size = maxQsize;
        } else if (len <= 0) {
            size = 1;
        } else {
            size = len;
        }
        putloc = getloc = 0;
    }

    void put(char ch) {
        if (putloc == size) {
            cout << " - Queue is full. Implementing a circular queue" << endl;
            putloc = 0; // wrap around
        }
        items[putloc++] = ch;
    }

    char get() {
        if (getloc == putloc) {
            cout << " - Queue is empty." << endl;
            return '\0';
        }
        return items[getloc++];
    }
}; 

