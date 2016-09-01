//============================================================================
// Name        : CSCI946-lab04-task2.cpp
// Author      : Baoxing Li
// Version     :
// Copyright   : bl251@uowmail.edu.au / sID: 5083497
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

int main() {
	cout << "Assignment" << endl; // prints Assignment
	int Dimension = 7;
	Matrix<double> aMatrix(Dimension, Dimension);
	double data[49] = { 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 2, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
			0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 2, 1, 0, 1 };
	aMatrix.setData(data, 49);
	cout << aMatrix;
	return 0;
}
