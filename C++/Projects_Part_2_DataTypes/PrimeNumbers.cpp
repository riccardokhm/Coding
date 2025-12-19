// Find the prime numbers up to a given limit using the Sieve of Eratosthenes algorithm

#include <iostream>


using namespace std;

int main(){
    int limit;
    cout << "Enter the upper limit to find prime numbers: ";
    cin >> limit;

    if (limit < 2) {
        cout << "There are no prime numbers less than 2." << endl;
        return 0;
    }

    bool* isPrime = new bool[limit + 1];
    for (int i = 0; i <= limit; i++) {
        isPrime[i] = true;
    }
    isPrime[0] = isPrime[1] = false; // 0 and 1 are not prime numbers

    for (int p = 2; p * p <= limit; p++) {
        if (isPrime[p]) {
            for (int multiple = p * p; multiple <= limit; multiple += p) {
                isPrime[multiple] = false;
            }
        }
    }

    cout << "Prime numbers up to " << limit << " are:" << endl;
    for (int i = 2; i <= limit; i++) {
        if (isPrime[i]) {
            cout << i << " ";
        }
    }
    cout << endl;

    delete[] isPrime;
    return 0;
}