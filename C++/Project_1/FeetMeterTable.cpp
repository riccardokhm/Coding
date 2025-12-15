// Conversion table from feet to meters //

#include <iostream>
using namespace std;

int main()
{
	double feet, meter;
	double limit;
	int counter = 0;

	cout << "Set the upper limit of the conversion table"<< endl;
	cin >> limit;

	for (feet = 1.0; feet < limit; ++feet)
	{
		meter = feet / 3.28;
		cout << feet << " feet equals " << meter << " meters"<< endl;

		counter++;
		if (counter == 10)
		{
			cout << endl;
			cout << endl;
			counter = 0;
		}
	}

	system("pause");
}