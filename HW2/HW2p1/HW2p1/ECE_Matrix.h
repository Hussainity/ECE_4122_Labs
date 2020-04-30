/* Author: Hussain Miyaziwala Class: ECE4122 Last Date Modified: 9/23/19

Description:

HW 2 Part 1 : ECE MATRIX CLASS

*/

#pragma once
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class ECE_Matrix;
ostream& operator<<(ostream& os, const ECE_Matrix & x);
double max(int, int);

class ECE_Matrix
{
public:
	int dimX, dimY;
	double* backArr;
	ECE_Matrix& operator/(double rhs) const;
	ECE_Matrix& operator*(double rhs) const;
	ECE_Matrix& operator+(double rhs) const;
	ECE_Matrix& operator-(double rhs) const;
	ECE_Matrix();
	ECE_Matrix(int squareDim, double init);
	ECE_Matrix(int rows, int columns, double init);
	ECE_Matrix(string f);
	ECE_Matrix& operator-(const ECE_Matrix& rhs) const;
	ECE_Matrix& operator+(const ECE_Matrix& rhs) const;
	ECE_Matrix& operator+=(const ECE_Matrix& rhs);
	ECE_Matrix& operator-=(const ECE_Matrix& rhs);
	ECE_Matrix& transpose() const;
	double operator()(int a, int b) const;
	~ECE_Matrix();
};

ECE_Matrix& operator+(double LHS, const ECE_Matrix& RHS);
ECE_Matrix& operator-(double LHS, const ECE_Matrix& RHS);
ECE_Matrix& operator*(double LHS, const ECE_Matrix& RHS);
ECE_Matrix& operator/(double LHS, const ECE_Matrix& RHS);

