#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <string>
#include <fstream>
#include "Matrix.cpp"

using namespace std;

class Matrix;
ostream& operator<< (ostream& os, const Matrix& x);
Matrix operator*(Matrix LHS, double RHS);

class Matrix {
public:
	int dimX, dimY;
	double* backingArray;
	bool err = false;

	Matrix(int, int);
	Matrix(int, int, double);

	Matrix& operator=(const Matrix& rhs);
	Matrix operator*(const Matrix& rhs);
	Matrix operator+(Matrix RHS);
	Matrix operator-(Matrix RHS);
	Matrix transpose();
};
#endif