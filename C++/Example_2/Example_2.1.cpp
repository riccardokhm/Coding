/* 
	Compute the area of a rectangle
*/

#include <iostream>
using namespace std;

int main(){
	int length;
	int width;

	cout << "Please provide the length" << endl;
	cin >> length;

	cout << "Please provide the width"<< endl;
	cin >> width;

	cout << "The rectangle area is " << width*length;

	cin.get();
	return 0;
}
