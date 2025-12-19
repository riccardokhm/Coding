// A simple project to compute the sum due monthly for a regular payment plan.

#include <iostream>
#include <cmath>

using namespace std;

int main(){
    double Amount, InterestRate, PaymentsPerYear, Years, MonthlyPayment;
    double numerator, denominator;
    double b, e;

    cout << "Enter the total amount to be paid: ";
    cin >> Amount;

    cout << "Enter the annual interest rate (in percentage): ";
    cin >> InterestRate;

    cout << "Enter the number of payments per year: ";
    cin >> PaymentsPerYear;

    cout << "Enter the total number of years for the payment plan: ";
    cin >> Years;


    #pragma region Calculate Monthly Payment

    numerator = InterestRate * Amount / PaymentsPerYear;

    e = -(PaymentsPerYear * Years);
    b = 1 + (InterestRate / PaymentsPerYear);

    denominator = 1 - pow(b, e);

    MonthlyPayment = numerator / denominator;
    
    #pragma endregion

    cout << "The monthly payment is: " << MonthlyPayment << endl;
    cout << "The total interest paid over the life of the loan is: " << (MonthlyPayment * PaymentsPerYear * Years) - Amount << endl;
    return 0;
    }