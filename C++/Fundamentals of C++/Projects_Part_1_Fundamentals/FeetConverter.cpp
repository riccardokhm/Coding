// Converter from feet to meters //

#include <iostream>
using namespace std;

int main()
{
	double feet = 0.0;
	double meter = 0.0;

	cout << "Please insert the length in feet: " << endl;
	cin >> feet;

	meter = feet * 3.28;
	cout << feet << " feet equals " << meter << " meters" << endl;

	system("pause");

	return 0;

}