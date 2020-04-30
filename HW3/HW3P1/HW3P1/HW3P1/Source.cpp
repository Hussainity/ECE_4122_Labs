/* Author: Hussain Miyaziwala Class: ECE4122 Last Date Modified: 10/5/19

Description: Homework 3, Problem 1

Note: Uses OMP for parallel processing, but doesn't actually improve run time given already optimized approach.
*/

#include <iostream>
#include <omp.h>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include<algorithm> 

using namespace std;

double countPaths(int sum, int dim1);

int main(int argc, char ** argv)
{
	if (argc != 5)
	{
		return 0;
	}	
	int height = atoi(argv[2]);
	int width = atoi(argv[4]);

	cout << "Number of routes: " << fixed << setprecision(0) << std::ceil(countPaths((height + width), max(height, width))) << endl;
}

double countPaths(int sum, int dim1)
{
	double product = 1;
	#pragma omp parallel for reduction(*: product)
	for (int i = dim1 + 1; i <= sum; i++)
	{
		product = product * double(i) / (sum - double(i) + 1);
	}
	   	return product;
}