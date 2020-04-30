/* Author: Hussain Miyaziwala Class: ECE4122 Last Date Modified: 9/23/19

Description:

HW 2 Part 1 : ECE MATRIX CLASS

*/

#include "ECE_Matrix.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

//max of two arguments is returned
double max(int a, int b)
{
	return (a < b) ? b : a;
}

ECE_Matrix::ECE_Matrix() :dimX(0), dimY(0)
{
	backArr = new double[0];
}
ECE_Matrix::~ECE_Matrix()
{
	delete[] backArr;
}

ECE_Matrix::ECE_Matrix(int squareDim, double init) :dimX(squareDim), dimY(squareDim)
{
	backArr = new double[dimX * dimY];
	for (int i = 0; i < dimX * dimY; i++)
	{
		backArr[i] = init;
	}
}

ECE_Matrix::ECE_Matrix(int row, int column, double init) :dimX(row), dimY(column)
{
	backArr = new double[dimX * dimY];
	for (int i = 0; i < dimX * dimY; i++)
	{
		backArr[i] = init;
	}
}

ECE_Matrix::ECE_Matrix(string f)
{
	ifstream inFile;
	inFile.open(f.c_str());
	double holder;
	inFile >> dimY;
	inFile >> dimX;
	backArr = new double[dimX * dimY];
	for (int i = 0; i < dimX * dimY; i++)
	{
		inFile >> holder;
		backArr[i] = holder;
	}
	inFile.close();
}

ostream& operator<< (ostream& os, const ECE_Matrix& x)
{

	for (int i = 0; i < x.dimX * x.dimY; i++)
	{
		if (((i % x.dimX) == 0) && (i != 0))
		{
			os << endl;
		}

		os << scientific << setw(12) << setprecision(3) << x.backArr[i];
	}

	return os;
}

double ECE_Matrix::operator()(int x, int y) const
{

	if ((x > dimX - 1) || (y > dimY - 1))
	{
		return 0;
	}

	return backArr[y * dimX + x];
}

ECE_Matrix& ECE_Matrix::operator-(const ECE_Matrix& rhs) const
{

	ECE_Matrix* a = new ECE_Matrix((int)max(dimX, rhs.dimX), (int)max(dimY, rhs.dimY), 0);

	for (int i = 0; i < a->dimX; i++)
	{
		for (int j = 0; j < a->dimY; j++)
		{
			a->backArr[j * a->dimX + i] = (*this)(i, j) - rhs(i, j);
		}
	}

	return *a;

}

ECE_Matrix& ECE_Matrix::operator-(double rhs) const
{
	ECE_Matrix* a = new ECE_Matrix(dimX, dimY, rhs * -1);
	for (int i = 0; i < dimX * dimY; i++)
	{
		a->backArr[i] += backArr[i];
	}
	return *a;
}

ECE_Matrix& ECE_Matrix::operator+(const ECE_Matrix& rhs) const
{

	ECE_Matrix* a = new ECE_Matrix((int)max(dimX, rhs.dimX), (int)max(dimY, rhs.dimY), 0);

	for (int i = 0; i < a->dimX; i++)
	{
		for (int j = 0; j < a->dimY; j++)
		{
			a->backArr[j * a->dimX + i] = (*this)(i, j) + rhs(i, j);
		}
	}

	return *a;

}

ECE_Matrix& ECE_Matrix::operator+(double rhs) const
{
	ECE_Matrix* a = new ECE_Matrix(dimX, dimY, rhs);
	for (int i = 0; i < dimX * dimY; i++)
	{
		a->backArr[i] += backArr[i];
	}
	return *a;
}

ECE_Matrix& ECE_Matrix::transpose() const
{

	ECE_Matrix* a = new ECE_Matrix(dimY, dimX, 0);

	for (int i = 0; i < dimX; i++)
	{
		for (int j = 0; j < dimY; j++)
		{
			a->backArr[dimY * i + j] = (*this)(i, j);
		}
	}
	return *a;
}

ECE_Matrix& ECE_Matrix::operator*(double RHS) const
{

	ECE_Matrix* a = new ECE_Matrix(dimX, dimY, 0);

	for (int i = 0; i < dimY * dimY; i++)
	{
		a->backArr[i] = backArr[i] * RHS;
	}

	return *a;
}

ECE_Matrix& ECE_Matrix::operator/(double RHS) const
{

	ECE_Matrix* a = new ECE_Matrix(dimX, dimY, 0);

	if (RHS == 0)
	{
		return *a;
	}

	for (int i = 0; i < dimY * dimY; i++)
	{
		a->backArr[i] = backArr[i] / RHS;
	}

	return *a;
}

ECE_Matrix& operator/(double LHS, const ECE_Matrix& RHS)
{
	return RHS / LHS;
}

ECE_Matrix& operator+(double LHS, const ECE_Matrix& RHS)
{
	return RHS + LHS;
}

ECE_Matrix& operator-(double LHS, const ECE_Matrix& RHS)
{
	return -1 * RHS + LHS;
}

ECE_Matrix& operator*(double LHS, const ECE_Matrix& RHS)
{
	return RHS.operator*(LHS);
}

ECE_Matrix& ECE_Matrix::operator+=(const ECE_Matrix& rhs)
{
	ECE_Matrix* a = new ECE_Matrix((int) max(dimX, rhs.dimX), (int)max(dimY, rhs.dimY), 0);

	for (int i = 0; i < a->dimX; i++)
	{
		for (int j = 0; j < a->dimY; j++)
		{
			a->backArr[j * a->dimX + i] = (*this)(i, j) + rhs(i, j);
		}
	}

	*this = *a;

	return *this;
}

ECE_Matrix& ECE_Matrix::operator-=(const ECE_Matrix& rhs)
{
	ECE_Matrix* a = new ECE_Matrix((int)max(dimX, rhs.dimX),(int) max(dimY, rhs.dimY), 0);

	for (int i = 0; i < a->dimX; i++)
	{
		for (int j = 0; j < a->dimY; j++)
		{
			a->backArr[j * a->dimX + i] = (*this)(i, j) - rhs(i, j);
		}
	}
	*this = *a;
	return *this;
}

