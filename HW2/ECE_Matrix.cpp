#include "ECE_Matrix.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

ECE_Matrix::ECE_Matrix(int squareDim, double init):dimX(squareDim),dimY(squareDim){
  backArr = new double[dimX*dimY];
  for (int i = 0; i < dimX * dimY; i++){
    backArr[i] = init;
  }
}

ECE_Matrix::ECE_Matrix(int row, int column, double init):dimX(row),dimY(column){
  backArr = new double[dimX*dimY];
  for (int i = 0; i < dimX * dimY; i++){
    backArr[i] = init;
  }
}

ECE_Matrix::ECE_Matrix(string f){
  ifstream inFile;
  inFile.open(f.c_str());
  double holder;
  inFile >> dimX;
  inFile >> dimY;
  backArr = new double[dimX*dimY];
  for (int i = 0; i < dimX * dimY; i++){
    inFile >> holder;
    backArr[i] = holder;
  }
  inFile.close();
}

int main(){
  ECE_Matrix M3 = ECE_Matrix("test.txt");

  for (int i = 0; i < M3.dimX * M3.dimY; i++){
    if (i % M3.dimX == 0){
      cout << endl;
    }
    cout << M3.backArr[i] << " ";
  }
  cout << endl;
}
