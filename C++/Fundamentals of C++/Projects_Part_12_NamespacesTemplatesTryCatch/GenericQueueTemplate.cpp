// Generate a class for a non-circular Queue data structure 
#include <iostream>
using namespace std;

const int maxQsize = 100; // maximum size of the Queue

namespace QueueCode{
    template <class QType> class Queue {
private:
    QType q[maxQsize];
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

    void put(QType data) {
        if (putloc == size) {
            cout << " - Queue is full." << endl;
            throw overflow_error;
        }
        q[putloc++] = data;
    }

    QType get() {
        if (getloc == putloc) {
            cout << " - Queue is empty." << endl;
            return underflow_error;
        }
        return q[getloc++];
    }
};
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

int main(){
    QueueCode::Queue<int> iQa(10), iQb(10);
    iQa.put(1);
    iQa.put(2);

    iQb.put(10);
    iQb.put(20);

    cout << "Content of the firts queue : ";
    for (int i = 0; i < 2; i++)
    {
        cout << iQa.get() << " ";
    }
    cout << endl;

    cout << "Content of the second queue : ";
    for (int i = 0; i < 2; i++)
    {
        cout << iQb.get() << " ";
    }
    cout << endl;
     
    system("pause");
    return 0;

}