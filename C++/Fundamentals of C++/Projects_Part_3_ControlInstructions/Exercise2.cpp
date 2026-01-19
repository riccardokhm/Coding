// Program to generate and visualize the progression 1,2,4,8,16,... up to a user-defined limit.
#include <iostream>
#include <iomanip>

using namespace std;

int main() {

    int limit;

    // Prompt user for the limit
    cout << "Enter the upper limit for the progression (power of 2): ";
    cin >> limit;

    cout << "Progression values up to " << limit << ":\n";

    // Generate and display the progression
    for (int value = 1; value <= limit; value *= 2) {
        cout << setw(10) << value << endl;
    }

    system("pause");
    return 0;
}