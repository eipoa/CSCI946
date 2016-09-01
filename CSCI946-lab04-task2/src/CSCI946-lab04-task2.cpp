//============================================================================
// Name        : CSCI946-lab04-task2.cpp
// Author      : Baoxing Li
// Version     :
// Copyright   : bl251@uowmail.edu.au / sID: 5083497
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Matrix.h"
using namespace std;

int main() {
	cout << "Assignment" << endl; // prints Assignment
	int Dimension = 7;
	Matrix<double> aMatrix(Dimension, Dimension);
	Matrix<double> tMatrix(Dimension, Dimension);
	double data[49] = { 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 2, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
			0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 2, 1, 0, 1 };
	aMatrix.setData(data, 49);
	cout << "   ------------------------ A Matrix -------------------------" << endl;
	cout << aMatrix << endl << endl;
	tMatrix = aMatrix.transpose();
	cout << "   ----------------------- AT Matrix -------------------------" << endl;
	cout << tMatrix << endl << endl;

	Matrix<double> aVextor(Dimension, 1);
	Matrix<double> hVextor(Dimension, 1);
	for (int i = 0; i < Dimension; ++i) {
		data[i] = 1;
	}
	hVextor.setData(data, 7);

	// begin calculate a, h
	Matrix<double> tmpa(Dimension, 1), tmph(Dimension, 1);
	aVextor = tMatrix * hVextor;
	hVextor = aMatrix * aVextor;
	aVextor.norm();
	hVextor.norm();
	cout << fixed << setprecision(3) << std::setiosflags(std::ios::right) << std::setfill(' ');
	cout << "      ----------------------- a/h Vector ------------------------" << endl;
	cout << "Va:";
	for (int j = 0; j < 7; ++j) {
		cout << std::setw(8) << aVextor(j, 0) << " ";
	}
	cout << endl;
	cout << "Vh:";
	for (int j = 0; j < 7; ++j) {
		cout << std::setw(8) << hVextor(j, 0) << " ";
	}
	cout << endl << endl;

	while (tmpa != aVextor || tmph != hVextor) {
		tmpa = aVextor;
		tmph = hVextor;
		aVextor = tMatrix * hVextor;
		hVextor = aMatrix * aVextor;
		aVextor.norm();
		hVextor.norm();
		cout << "      ----------------------- a/h Vector ------------------------" << endl;
		cout << "Va:";
		for (int j = 0; j < 7; ++j) {
			cout << std::setw(8) << aVextor(j, 0) << " ";
		}
		cout << endl;
		cout << "Vh:";
		for (int j = 0; j < 7; ++j) {
			cout << std::setw(8) << hVextor(j, 0) << " ";
		}
		cout << endl << endl;
	}
	return 0;
}
