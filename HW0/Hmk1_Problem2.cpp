/*
Author: Hussain Miyaziwala
Class: ECE4122
Last Date Modified: 9/1/2019
Description:
Solution to Numerical Integration (HW1.2)
Compile with g++
Assumes safe input
*/

#include <iostream>

using namespace std;

int main()
{
	double n;
	unsigned long div;
	cout << "Enter the value for the upper limit:" << endl;
	cin >> n;
	cout << "Enter the number of divisions:" << endl;
	cin >> div;

	double value = 0;
	double step = n / div;

	for (double i = 0; i < n; i = i + step)
	{
		value += (4 / (1 + (i * i))); //F(x) calculation preformed
	}

	cout << value / div << endl;
}
