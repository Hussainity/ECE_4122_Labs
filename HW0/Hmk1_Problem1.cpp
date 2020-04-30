/*
Author: Hussain Miyaziwala
Class: ECE4122
Last Date Modified: 9/1/2019
Description:
Solution to oneMoreOne (HW1.1)
Compile with g++
Assumes safe input
*/

#include <iostream>

using namespace std;

int oneMoreOne(int,int);

int main()
{
	int n;
	cout << "Please enter the starting number n:" << endl;
	cin >> n;

	int solution = oneMoreOne(n, 0);

	cout << "The sequence had " << solution << " instances of 1 added." << endl;
}

/*
Recursive function for HW Solution.
*/
int oneMoreOne(int n, int count)
{
	if (n == 1)
	{
		return count;
	}
	if (n % 7 == 0)
	{
		return oneMoreOne(n / 7, count);
	}
	return oneMoreOne(n + 1, count + 1);
}
