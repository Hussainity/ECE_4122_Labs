#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//ostream& operator<<(ostream& os, const ECE_Matrix & x);

class ECE_Matrix{
public:
  int dimX, dimY;
  double * backArr;
  ECE_Matrix(int squareDim, double init);
  ECE_Matrix(int rows, int columns, double init);
  ECE_Matrix(string f);
};
