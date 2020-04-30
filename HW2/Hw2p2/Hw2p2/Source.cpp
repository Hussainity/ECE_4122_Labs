/* Author: Hussain Miyaziwala Class: ECE4122 Last Date Modified: 10/5/19

Description:

HW 3 Part 2 : MAX PRODUCT

*/

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "prodBuff.h"
#include <omp.h>

using namespace std;
double** backArr;
int dimY, dimX;

double* soln;

void horizontal(int);
void vertical(int);
void diagonalL2R(int);
void diagonalR2L(int);

int main(int argc, char** argv)
{

	if (argc != 2)
	{
		return 1;
	}
	string f = argv[1];

	ifstream inFile;
	inFile.open(f.c_str());
	double holder;

	inFile >> dimY;
	inFile >> dimX;

	//initialize array
	backArr = new double* [dimX];

	for (int i = 0; i < dimX; i++)
	{
		backArr[i] = new double[dimY];
	}

	for (int j = 0; j < dimY; j++)
	{
		for (int i = 0; i < dimX; i++)
		{
			inFile >> holder;
			backArr[i][j] = holder;
		}
	}
	inFile.close();

	//organized for threading, but not implemented so

	soln = new double[4];

	#pragma omp parallel sections default(none) shared(soln)
	{
		horizontal(0);
	#pragma omp section
		vertical(1);
	#pragma omp section
		diagonalL2R(2);
	#pragma omp section
		diagonalR2L(3);
	}


	double max_value = soln[0];
	for (int i = 1; i < 4; i++)
	{
		max_value = soln[i] > max_value ? soln[i] : max_value;
	}

	ofstream outFile;
	outFile.open("output2.txt");

	outFile << setprecision(10) << max_value;

	for (int i = 0; i < dimX; i++)
	{
		delete[] backArr[i];
	}

	delete[] backArr;

	delete[] soln;

	return 0;
}

void horizontal(int a) {

	prodBuff X;

	for (int j = 0; j < dimY; j++)
	{
		for (int i = 0; i < dimX; i++)
		{
			X.process(backArr[i][j]);
		}
		X.reset();
	}
	soln[a] = X.MAX;
}

void vertical(int a)
{

	prodBuff X;

	for (int j = 0; j < dimX; j++)
	{
		for (int i = 0; i < dimY; i++)
		{
			X.process(backArr[j][i]);
		}
		X.reset();
	}
	soln[a] = X.MAX;
}

void diagonalL2R(int a)
{
	prodBuff X;
	int i;

	for (int k = 0; k <= dimX + dimY - 2; k++)
	{
		for (int j = 0; j <= k; j++)
		{
			i = k - j;
			if (i < dimY && j < dimX)
			{
				X.process(backArr[i][j]);
			}
		}
		X.reset();
	}

	soln[a] = X.MAX;
}

void diagonalR2L(int a)
{
	prodBuff X;

	//xdir
	for (int k = 0; k < dimX; k++)
	{
		int j = 0;
		for (int i = k; i < dimX; i++)
		{
			X.process(backArr[i][j]);
			j++;
			if (j >= dimY)
			{
				break;
			}
		}
		X.reset();
	}

	for (int k = 0; k < dimY; k++)
	{
		int i = 0;
		for (int j = k; j < dimY; j++)
		{
			X.process(backArr[i][j]);
			i++;
			if (i >= dimX)
			{
				break;
			}
		}
		X.reset();
	}

	soln[a] = X.MAX;
}

