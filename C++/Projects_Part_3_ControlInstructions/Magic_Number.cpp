// Program which mimic the behaviour of a user trying to guess a magic number
#include <iostream>
#include <cstdlib>
#include <random>
using namespace std;

int main()
{
    random_device rd; // Obtain a random number from hardware
    mt19937 eng(rd()); // Seed the generator
    uniform_int_distribution<> distr(1, 100); // Define the range
    const int MAGIC_NUMBER = distr(eng); // The magic number to guess
    int userGuess = 0; // Variable to store user's guess

    cout << "Welcome to the Magic Number Guessing Game!" << endl;
    cout << "Try to guess the magic number between 1 and 100." << endl;

    // Loop until the user guesses the correct number
    while (userGuess != MAGIC_NUMBER)
    {
        cout << "Enter your guess: ";
        cin >> userGuess;

        if (userGuess < MAGIC_NUMBER)
        {
            cout << "Too low! Try again." << endl;
        }
        else if (userGuess > MAGIC_NUMBER)
        {
            cout << "Too high! Try again." << endl;
        }
        else
        {
            cout << "Congratulations! You've guessed the magic number!" << endl;
        }
    }

    system("pause");
}

