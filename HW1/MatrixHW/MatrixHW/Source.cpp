#include <iostream>
#include <limits>
#include "Matrix.h"



int main() 
{

    //const Matrix M1(10, 20) = 5.0;  // Construct 10 x 20 array set all elements to 5.0
    const Matrix M2(20, 10, 200.0); // Construct 20 x 10 array, setting all element to 200.0
	/*const Matrix M3("data_matrix.txt"); // Construct matrix from file

    std::cout << "M3: " << std::endl << M1 << std::endl;

    // Doing some simple matrix math
    const Matrix M4 = M1 - M2;
    const Matrix M5 = M3 - M2;
    const Matrix M6 = M1 + M2;
    const Matrix M7 = M3.Transpose();   // Function should not Modify M3 directly
                                            // but just return the Transpose of the matrix

    const Matrix M9 = M1 * 8.0;
    const Matrix M10 = M1 / 8.0;

    Matrix M11; // Default constructor size 0 x 0
    M11 += M1;
    Matrix M12;
    M12 -= M1;

    const Matrix M13 = M1 + 8.0;
    const Matrix M14 = M1 - 8.0;

    const Matrix M15 = 8.0 * M1;
    const Matrix M17 = 8.0 + M1;
    const Matrix M18 = 8.0 - M1;

    std::cout << "M4: (M1 - M2): " << std::endl << M4 << std::endl;
    std::cout << "M5: (M3 - M2) " << std::endl << M5 << std::endl;
    std::cout << "M6: (M1 + M2) " << std::endl << M6 << std::endl;
    std::cout << "M7: M3 Transposed " << std::endl << M6 << std::endl;
    std::cout << "M9: (M1 * 8.0) " << std::endl << M9 << std::endl;
    std::cout << "M10: (M1 / 8.0) " << std::endl << M10 << std::endl;
    std::cout << "M13: (M1 + 8.0) " << std::endl << M13 << std::endl;
    std::cout << "M14: (M1 - 8.0) " << std::endl << M14 << std::endl;
    std::cout << "M15: (8.0 * M1) " << std::endl << M15 << std::endl;
    std::cout << "M17: (8.0 + M1) " << std::endl << M17 << std::endl;
    std::cout << "M18: (8.0 - M1) " << std::endl << M18 << std::endl;
	*/
 }
